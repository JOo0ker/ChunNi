#pragma once
#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "ElaCheckBox.h"

#define INDEX_STRING(T1, T2, V)\
	T1[T2::##V]

#define TODO_STR(N) INDEX_STRING(TO_DO_JSON_KEY_LIST, ToDoJsonKey, N)

namespace CN
{
	enum DayOfWeekBit
	{
		EveryDay = 1 << 0,
		Monday = 1 << 1,
		Tuesday = 1 << 2,
		Wednesday = 1 << 3,
		Thursday = 1 << 4,
		Friday = 1 << 5,
		Saturday = 1 << 6,
		Sunday = 1 << 7
	};

	enum TimePointKey
	{
		DayOfWeek,
		Time,

		Max
	};

	enum ToDoJsonKey
	{
		Name,
		LastUpdateTime,
		ResetTimes,
		State
	};

	const QStringList TO_DO_JSON_KEY_LIST{
		"itemName","lastUpdateTime", "resetTimes", "state"
	};

	struct TimePoint
	{
		Qt::DayOfWeek DayOfWeek{ Qt::Monday };
		QTime Time{ 0,0 };

		static TimePoint CreateOne(QJsonArray array)
		{
			TimePoint tp;
			if (array.size() < TimePointKey::Max)
			{
				qWarning() << "TimePoint Load array size error.";
				return tp;
			}

			tp.DayOfWeek = static_cast<Qt::DayOfWeek>(array[TimePointKey::DayOfWeek].toInt());
			tp.Time = QTime::fromString(array[TimePointKey::Time].toString(), Qt::ISODate);

			return tp;
		}

		bool operator==(const TimePoint& r) const
		{
			return DayOfWeek == r.DayOfWeek && Time == r.Time;
		}

		QJsonArray Save() { return { DayOfWeek, Time.toString(Qt::ISODate) }; }
	};


	class ToDoItemBase
	{
	public:
		ToDoItemBase() = default;
		virtual ~ToDoItemBase()  = default;
		virtual void CheckItem() = 0;
		virtual void RestItem() = 0;
		virtual void SetRestTimeWeek(QVector<TimePoint>) = 0;
		virtual void Load(QJsonObject&) = 0;
		virtual QJsonObject Save() = 0;
	};


	class ToDoItemElaCheckBox : public ElaCheckBox, public ToDoItemBase
	{
		Q_OBJECT
	public:
		explicit ToDoItemElaCheckBox(QWidget* parent = nullptr);
		explicit ToDoItemElaCheckBox(const QString& itemName, QVector<TimePoint> timePoint, QWidget* parent = nullptr);

		void CheckItem() override;
		void RestItem() override;
		void SetRestTimeWeek(QVector<TimePoint> timePoints) override;

		void Load(QJsonObject& object) override;
		QJsonObject Save() override;

		QDateTime CalculateNearestTimePoint(const QDateTime& dateTime);

	private:
		QString ItemName{};

		QVector<TimePoint> ResetTimePoints;
	};
}