#include "ToDo/ToDoItemElaCheckBox.h"

#include <QJsonArray>

namespace CN
{
	ToDoItemElaCheckBox::ToDoItemElaCheckBox(QWidget* parent)
		: ElaCheckBox(parent)
	{}

	ToDoItemElaCheckBox::ToDoItemElaCheckBox(const QString& itemName, const QVector<TimePoint> timePoint, QWidget* parent)
		: ElaCheckBox(itemName, parent)
		  , ItemName(itemName)
		  , ResetTimePoints(timePoint)
	{}

	void ToDoItemElaCheckBox::CheckItem()
	{ setChecked(true); }

	void ToDoItemElaCheckBox::RestItem()
	{ setChecked(false); }

	void ToDoItemElaCheckBox::SetRestTimeWeek(const QVector<TimePoint> timePoints)
	{ ResetTimePoints = timePoints; }

	void ToDoItemElaCheckBox::Load(QJsonObject& object)
	{
		// Item Name
		//------------------------------
		ItemName = object[TODO_STR(Name)].toString();
		setText(ItemName);

		// Current state
		//------------------------------
		if(const auto lastUpdateTime = QDateTime::fromString(object[TODO_STR(LastUpdateTime)].toString(), Qt::ISODate); 
			lastUpdateTime.isValid() && !lastUpdateTime.isNull())
		{// Got lastUpdateTime from file.
			if(QDateTime::currentDateTime() > CalculateNearestTimePoint(lastUpdateTime))
			{// current time is after the nearest update time point.
				RestItem();
			}
			else
			{// load old state
				setChecked(object[TODO_STR(State)].toBool());
			}
		}
		else
		{// default set
			setChecked(false);
		}

		// Reset times
		//------------------------------
		auto resetTimes = object[TODO_STR(ResetTimes)].toArray();
		ResetTimePoints.clear();
		for(auto time : resetTimes)
		{
			ResetTimePoints.append(TimePoint::CreateOne(time.toArray()));
		}
	}

	QJsonObject ToDoItemElaCheckBox::Save()
	{
		QJsonObject obj;
		obj[TODO_STR(Name)] = ItemName;
		obj[TODO_STR(LastUpdateTime)] = QDateTime::currentDateTime().toString(Qt::ISODate);
		QJsonArray resetTimesArray;
		for(auto& timePoint : ResetTimePoints)
		{
			resetTimesArray.append(timePoint.Save());
		}
		obj[TODO_STR(ResetTimes)] = resetTimesArray;
		obj[TODO_STR(State)] = isChecked();

		return obj;
	}

	QDateTime ToDoItemElaCheckBox::CalculateNearestTimePoint(const QDateTime& dateTime)
	{
		QDateTime nearestDateTime;
		int minDiff = std::numeric_limits<int>::max();

		for (const auto& [dayOfWeek, time] : ResetTimePoints)
		{
			QDateTime targetDateTime(dateTime.date(), time);

			// 调整目标时间点的日期
			int daysDiff = static_cast<int>(dayOfWeek) - static_cast<int>(dateTime.date().dayOfWeek());
			if (daysDiff < 0 || (daysDiff == 0 && time <= dateTime.time()))
			{
				daysDiff += 7; // 如果时间点已经过去，调整到下一周
			}
			targetDateTime = targetDateTime.addDays(daysDiff);

			if (const int diff = static_cast<int>(dateTime.msecsTo(targetDateTime));
				diff < minDiff && diff > 0)
			{ // 确保差值为正
				minDiff = diff;
				nearestDateTime = targetDateTime;
			}
		}

		return nearestDateTime;
	}
}
