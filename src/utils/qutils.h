#ifndef QUTILS_H
#define QUTILS_H

#include <QList>
#include <QLineEdit>
#include <QWidget>
#include <QSqlRecord>
#include <initializer_list>

namespace UiUtils {

    void setFieldsEnabled(bool enabled, const std::initializer_list<QWidget*> fields){
        for(QWidget* field : fields){
            field->setEnabled(enabled);
        }
    }

    bool validateLineEdits(QList<QLineEdit*> list){
        if(list.isEmpty()) return false;

        for(QLineEdit* lineEdit : list){
            if(lineEdit && lineEdit->text().isEmpty()){
                return false;
            }
        } return true;
    }

    void setLineEditsText(QList<QLineEdit*> list, QSqlRecord data = QSqlRecord()){
        if(data.isEmpty()){
            for (QLineEdit* line : list) line->clear();
        } else {
            for (int i=0; i < list.size(); ++i)
                list[i]->setText(data.value(i).toString());
        }
    }
}

#endif // QUTILS_H
