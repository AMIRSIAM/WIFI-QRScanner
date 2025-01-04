#pragma once
#include <QQmlEngine>
#include <QVideoFrame>

#include "FrameProcessor.h"

#include "ReadBarcode.h"

namespace QrScanner
{
	struct WiFi
	{
		bool isWiFi;
		std::string type;
		std::string password;
		std::string ssid;
		bool hidden;
	};

	class Driver : public QObject
	{
		Q_OBJECT
		QML_ELEMENT
		QML_SINGLETON

	public:
		enum ConnectingState
		{
			NotConnected,
			Connecting,
			Connected
		};
		Q_ENUM(ConnectingState);

		Driver(QObject *parent = nullptr);
		~Driver();

		Q_INVOKABLE void setVideoSink(QVideoSink *sink);
		void scanBarcode(const QVideoFrame &frame);
		void processScanResult(const ZXing::Result result);
		WiFi parseWiFiCredentials(const std::string& wifiCredentials);
		bool connectWiFi(const QString &ssid);
		Q_INVOKABLE void connectToWifi(const QString &ssid, const QString &password);

	signals:
		void wifiNetworkFound(const QString &ssid, const QString &password);
		void connectingStateChanged(ConnectingState state);

	private:
		FrameProcessor *m_frameProcessor;
		ConnectingState m_connectingState = NotConnected;
	};
}
