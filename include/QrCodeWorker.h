#include <QObject>
#include <QImage>
#include <QRunnable>
#include <ReadBarcode.h>
#include <QDebug>

namespace QrScanner
{
	class BarcodeWorker : public QObject
	{
		Q_OBJECT
	public:
		BarcodeWorker(QObject *parent = nullptr);
		~BarcodeWorker();

		ZXing::Result processImage(const QImage image);

	signals:
		void imageProcessed(const ZXing::Result &result);
	};

	class BarcodeTask : public QRunnable
	{
	public:
		BarcodeTask(const QImage image) : mImage(image) { Worker = new BarcodeWorker(); }

		void run() override;

		BarcodeWorker* Worker;

	private:
		QImage mImage;
	};
}
