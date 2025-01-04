#include "QrCodeWorker.h"

namespace QrScanner
{
	inline static ZXing::ImageFormat
	ImgFmtFromQImg(const QImage& img)
	{
		switch (img.format())
		{
			case QImage::Format_ARGB32:
			case QImage::Format_RGB32:
	#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				return ZXing::ImageFormat::BGRX;
	#else
				return ZXing::ImageFormat::XRGB;
	#endif
			case QImage::Format_RGB888: return ZXing::ImageFormat::RGB;
			case QImage::Format_RGBX8888:
			case QImage::Format_RGBA8888: return ZXing::ImageFormat::RGBX;
			case QImage::Format_Grayscale8: return ZXing::ImageFormat::Lum;
			default: return ZXing::ImageFormat::None;
		}
	}

	BarcodeWorker::BarcodeWorker(QObject *parent)
		: QObject(parent)
	{
	}
	BarcodeWorker::~BarcodeWorker()
	{
	}

	ZXing::Result BarcodeWorker::processImage(const QImage image)
	{
		auto fmt = ImgFmtFromQImg(image);

		const auto readerOptions = ZXing::ReaderOptions()
									.setFormats(ZXing::BarcodeFormat::QRCode)
									.setTryHarder(true)
									.setTryRotate(true)
									.setIsPure(false)
									.setBinarizer(ZXing::Binarizer::LocalAverage);

		if (fmt == ZXing::ImageFormat::None)
		{
			QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
			fmt = ImgFmtFromQImg(grayImage);
			auto result = ZXing::ReadBarcode({grayImage.bits(), grayImage.width(), grayImage.height(), fmt}, readerOptions);
			emit imageProcessed(result);
			return result;
		}
		else
		{
			auto result = ZXing::ReadBarcode({image.bits(), image.width(), image.height(), fmt}, readerOptions);;
			emit imageProcessed(result);
			return result;
		}
	}

	void BarcodeTask::run()
	{
		Worker->processImage(mImage);
	}
}