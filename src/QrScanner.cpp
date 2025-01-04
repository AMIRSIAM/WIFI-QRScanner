#include "QrScanner.h"
#include "QrCodeWorker.h"

#include <QImage>
#include <QThreadPool>

#include <sstream>
#include <QProcess>
#include <QFile>


namespace QrScanner
{
	Driver::Driver(QObject *parent)
		: QObject(parent)
	{
		m_frameProcessor = new FrameProcessor(parent);
		connect(m_frameProcessor, &FrameProcessor::frameReady, this, &Driver::scanBarcode);
		QThreadPool::globalInstance()->setMaxThreadCount(10);
	}

	Driver::~Driver()
	{
		QThreadPool::globalInstance()->clear();
	}

	void Driver::setVideoSink(QVideoSink *sink)
	{
		m_frameProcessor->setVideoSink(sink);
	}

	inline static void
	createWiFiProfile(const QString &ssid, const QString &password)
	{
		// Create the XML profile content
		QString xmlProfile = QString(
				"<?xml version=\"1.0\"?>"
					"<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">"
						"<name>%1</name>"
						"<SSIDConfig>"
							"<SSID>"
								"<name>%1</name>"
							"</SSID>"
						"</SSIDConfig>"
						"<connectionType>ESS</connectionType>"
						"<connectionMode>auto</connectionMode>"
						"<MSM>"
							"<security>"
								"<authEncryption>"
									"<authentication>WPA2PSK</authentication>"
									"<encryption>AES</encryption>"
									"<useOneX>false</useOneX>"
								"</authEncryption>"
								"<sharedKey>"
									"<keyType>passPhrase</keyType>"
									"<protected>false</protected>"
									"<keyMaterial>%2</keyMaterial>"
								"</sharedKey>"
							"</security>"
						"</MSM>"
					"</WLANProfile>"
				).arg(ssid, password);

		// Save the XML profile to a file
		QString profileFileName = ssid + ".xml"; // e.g., Your_SSID.xml
		QFile profileFile(profileFileName);
		if (profileFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			profileFile.write(xmlProfile.toUtf8());
			profileFile.close();
		} else {
			qDebug() << "Failed to create profile file.";
			return;
		}

		// Use netsh to add the profile
		QProcess addProfileProcess;
		QString addProfileCommand = QString("netsh wlan add profile filename=\"%1\"").arg(profileFileName);
		addProfileProcess.start(addProfileCommand);
		addProfileProcess.waitForFinished();

		QString addProfileOutput = addProfileProcess.readAllStandardOutput();
		QString addProfileError = addProfileProcess.readAllStandardError();

		if (!addProfileError.isEmpty())
			qDebug() << "Add Profile Error:" << addProfileError;
	}

	bool Driver::connectWiFi(const QString &ssid)
	{
		QProcess process;

		// Command to connect to Wi-Fi
		QString command = QString("netsh wlan connect name=\"%1\"").arg(ssid);

		process.startCommand(command);
		process.waitForFinished();

		// Check if the connection was successful
		//[TODO] make sure it is connected to this network
		if (process.exitCode() == 0)
		{
			m_connectingState = Connected;
			emit connectingStateChanged(m_connectingState);
			return true;
		}
		else
		{
			m_connectingState = NotConnected;
			emit connectingStateChanged(m_connectingState);
			qDebug() <<"Failed to connect to Wi-fi : "<< process.readAllStandardError();
			return false;
		}

	}

	void Driver::scanBarcode(const QVideoFrame &frame)
	{
		auto image = frame.toImage();

		BarcodeTask *task = new BarcodeTask(image);

		connect(task->Worker, &BarcodeWorker::imageProcessed, this, &Driver::processScanResult);

		// Start the task in the thread pool
		QThreadPool::globalInstance()->start(task);
	}

	void Driver::processScanResult(const ZXing::Result result)
	{
		if (result.isValid())
		{
			auto wifi = parseWiFiCredentials(result.text());
			if (wifi.isWiFi)
			{
				emit wifiNetworkFound(wifi.ssid.c_str(), wifi.password.c_str());
			}
		}
	}

	WiFi Driver::parseWiFiCredentials(const std::string& wifiCredentials)
	{
		WiFi wifi;

		// Check if the string starts with "WIFI:"
		if (wifiCredentials.substr(0, 5) != "WIFI:")
			return wifi;

		wifi.isWiFi = true;
		// Remove the "WIFI:" prefix
		auto credentials = wifiCredentials.substr(5);

		std::string token;
		std::istringstream stream(credentials);

		while (std::getline(stream, token, ';'))
		{
			if (token.substr(0, 2) == "T:")
				wifi.type = token.substr(2);
			else if (token.substr(0, 2) == "P:")
				wifi.password = token.substr(2);
			else if (token.substr(0, 2) == "S:")
				wifi.ssid = token.substr(2);
			else if (token.substr(0, 2) == "H:")
				wifi.hidden = (token.substr(2) == "true");
		}

		return wifi;
	}

	void Driver::connectToWifi(const QString &ssid, const QString &password)
	{
		m_connectingState = Connecting;
		emit connectingStateChanged(m_connectingState);
		createWiFiProfile(ssid, password);
		connectWiFi(ssid);
	}

} // namespace QrScanner