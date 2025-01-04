#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>

#include "QRScanner.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setFont(QFont("DM Sans"));
	app.setApplicationName("QRScanner");


	QQmlApplicationEngine engine;
	engine.addImportPath("qrc:/");

	auto driver = engine.singletonInstance<QrScanner::Driver*>("QrScanner", "Driver");

	const QUrl url(u"qrc:/QRScanner/main.qml"_qs);
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
