/**
 * @file ContainerConfig.cpp
 * @brief Libertine Manager containers configuration module
 */
/*
 * Copyright 2015 Canonical Ltd
 *
 * Libertine is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * Libertine is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "libertine/ContainerConfig.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <stdexcept>


namespace {
  /**
   * Extracts the container id from a JSON object.
   *
   * The container id must exist and be non-empty.
   */
  QString
  extract_container_id_from_json(QJsonObject const& json_object)
  {
    QJsonValue value = json_object["id"];
    if (value == QJsonValue::Undefined)
    {
        throw std::runtime_error("container id not found in JSON object");
    }

    QJsonValue::Type value_type = value.type();
    if (value_type != QJsonValue::String)
    {
        throw std::runtime_error("container id not valid type in JSON object");
    }

    QString id = value.toString();
    if (id.length() == 0)
    {
        throw std::runtime_error("container id empty in JSON object");
    }

    return id;
  }

  QString
  extract_container_name_from_json(QJsonObject const& json_object,
                                   QString const&     container_id)
  {
    QString name = container_id;
    QJsonValue value = json_object["name"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        QString s = value.toString();
        if (s.length() > 0)
        {
          name = s;
        }
      }
    }
    
    return name;
  }

  QString
  extract_container_type_from_json(QJsonObject const& json_object,
                                   QString const&     container_id)
  {
    QString type = container_id;
    QJsonValue value = json_object["type"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        QString s = value.toString();
        if (s.length() > 0)
        {
          type = s;
        }
      }
    }

    return type;
  }

  QString
  extract_image_id_from_json(QJsonObject const& json_object,
                             QString const&     container_id)
  {
    QString image = container_id;
    QJsonValue value = json_object["image"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        QString s = value.toString();
        if (s.length() > 0)
        {
          image = s;
        }
      }
    }

    return image;
  }

  const static struct { QString string; ContainerConfig::InstallStatus enumeration; } install_status_names[] =
  {
    { "new",          ContainerConfig::InstallStatus::New         },
    { "downloading",  ContainerConfig::InstallStatus::Downloading },
    { "configuring",  ContainerConfig::InstallStatus::Configuring },
    { "ready",        ContainerConfig::InstallStatus::Ready       },
    { QString(),      ContainerConfig::InstallStatus::New         }
  };

  ContainerConfig::InstallStatus
  extract_install_status_from_json(QJsonObject const& json_object)
  {
    QJsonValue value = json_object["installStatus"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        QString s = value.toString();
        if (s.length() > 0)
        {
          for (auto const& name: install_status_names)
          {
            if (0 == s.compare(name.string, Qt::CaseInsensitive))
            {
              return name.enumeration;
            }
          }
        }
      }
    }
    return ContainerConfig::InstallStatus::New;
  }

  const static struct { QString string; ContainerApps::AppStatus enumeration; } app_status_names[] =
  {
    { "new",        ContainerApps::AppStatus::New        },
    { "installing", ContainerApps::AppStatus::Installing },
    { "installed",  ContainerApps::AppStatus::Installed  },
    { "failed",     ContainerApps::AppStatus::Failed     },
    { QString(),    ContainerApps::AppStatus::New        }
  };

  QString
  extract_package_name_from_json(QJsonObject const& json_object)
  {
    QString package_name;

    QJsonValue value = json_object["packageName"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        package_name = value.toString();
      }
    }

    return package_name;
  }

  ContainerApps::AppStatus
  extract_app_status_from_json(QJsonObject const& json_object)
  {
    ContainerApps::AppStatus app_status = ContainerApps::AppStatus::New;

    QJsonValue value = json_object["appStatus"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        QString s = value.toString();
        if (s.length() > 0)
        {
          for (auto const& name: app_status_names)
          {
            if (0 == s.compare(name.string, Qt::CaseInsensitive))
            {
              app_status = name.enumeration;
              break;
            }
          }
        }
      }
    }

    return app_status;
  }

  QString
  extract_package_version_from_json(QJsonObject const& json_object)
  {
    QString version;

    QJsonValue value = json_object["version"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        version = value.toString();
      }
    }

    return version;
  }

  QString
  extract_package_maintainer_from_json(QJsonObject const& json_object)
  {
    QString maintainer;

    QJsonValue value = json_object["maintainer"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        maintainer = value.toString();
      }
    }

    return maintainer;
  }

  QString
  extract_package_description_from_json(QJsonObject const& json_object)
  {
    QString description;

    QJsonValue value = json_object["description"];
    if (value != QJsonValue::Undefined)
    {
      QJsonValue::Type value_type = value.type();
      if (value_type == QJsonValue::String)
      {
        description = value.toString();
      }
    }

    return description;
  }

  QList<ContainerApps*>
  extract_container_apps_from_json(QJsonObject const& json_object)
  {
    QList<ContainerApps*> container_apps;
    QString package_name,
            version,
            maintainer,
            description;
    ContainerApps::AppStatus app_status;

    QJsonArray installed_apps = json_object["installedApps"].toArray();

    for (auto const& app: installed_apps)
    {
      package_name = extract_package_name_from_json(app.toObject());
      app_status = extract_app_status_from_json(app.toObject());
      version = extract_package_version_from_json(app.toObject());
      maintainer = extract_package_maintainer_from_json(app.toObject());
      description = extract_package_description_from_json(app.toObject());

      container_apps.append(new ContainerApps(package_name, app_status, version, maintainer, description));
    }
    return container_apps;
  }
} // anonymous namespace


ContainerApps::
ContainerApps(QString const& package_name,
              ContainerApps::AppStatus app_status,
              QObject* parent)
: QObject(parent)
, package_name_(package_name)
, app_status_(app_status)
{ }


ContainerApps::
ContainerApps(QString const& package_name,
              ContainerApps::AppStatus app_status,
              QString const& version,
              QString const& maintainer,
              QString const& description,
              QObject* parent)
: QObject(parent)
, package_name_(package_name)
, app_status_(app_status)
, version_(version)
, maintainer_(maintainer)
, description_(description)
{ }


ContainerApps::
~ContainerApps()
{ }


QString const& ContainerApps::
package_name() const
{ return package_name_; }


ContainerApps::AppStatus ContainerApps::
app_status() const
{ return app_status_; }


void ContainerApps::
app_status(AppStatus app_status)
{
  if (app_status != app_status_)
  {
    app_status_ = app_status;
  }
}


QString const& ContainerApps::
version() const
{ return version_; }


void ContainerApps::
version(QString const& version)
{
  if (version != version_)
  {
    version_ = version;
  }
}


QString const& ContainerApps::
maintainer() const
{ return maintainer_; }


void ContainerApps::
maintainer(QString const& maintainer)
{
  if (maintainer != maintainer_)
  {
    maintainer_ = maintainer;
  }
}


QString const& ContainerApps::
description() const
{ return description_; }


void ContainerApps::
description(QString const& description)
{
  if (description != description_)
  {
    description_ = description;
  }
}


ContainerConfig::
ContainerConfig(QObject* parent)
: QObject(parent)
{ }


ContainerConfig::
ContainerConfig(QString const& container_id,
                QString const& container_name,
                QString const& container_type,
                QString const& image_id,
                QObject*       parent)
: QObject(parent)
, container_id_(container_id)
, container_name_(container_name)
, container_type_(container_type)
, image_id_(image_id)
, install_status_(InstallStatus::New)
{ }



ContainerConfig::
ContainerConfig(QJsonObject const& json_object,
                QObject*          parent)
: QObject(parent)
, container_id_(extract_container_id_from_json(json_object))
, container_name_(extract_container_name_from_json(json_object, container_id_))
, container_type_(extract_container_type_from_json(json_object, container_id_))
, image_id_(extract_image_id_from_json(json_object, container_id_))
, install_status_(extract_install_status_from_json(json_object))
, container_apps_(extract_container_apps_from_json(json_object))
{ }


ContainerConfig::
~ContainerConfig()
{ }


QString const& ContainerConfig::
container_id() const
{ return container_id_; }


QString const& ContainerConfig::
name() const
{ return container_name_; }


void ContainerConfig::
name(QString const& name)
{
  container_name_ = name;
  emit nameChanged();
}


QString const& ContainerConfig::
container_type() const
{ return container_type_; }


QString const& ContainerConfig::
image_id() const
{ return image_id_; }


ContainerConfig::InstallStatus ContainerConfig::
install_status() const
{ return install_status_; }


void ContainerConfig::
install_status(InstallStatus install_status)
{
  install_status_ = install_status;
  emit installStatusChanged();
}


QList<ContainerApps*> & ContainerConfig::
container_apps()
{ return container_apps_; }


QJsonObject ContainerConfig::
toJson() const
{
  QJsonObject json_object,
              app_object;
  QJsonArray apps;

  json_object["id"] = container_id_;
  json_object["name"] = container_name_;
  json_object["type"] = container_type_;
  json_object["image"] = image_id_;
  for (auto const& name: install_status_names)
  {
    if (install_status_ == name.enumeration)
    {
      json_object["installStatus"] = name.string;
      break;
    }
  }
  for (auto const& container_app: container_apps_)
  {
    app_object["packageName"] = container_app->package_name();
    app_object["version"] = container_app->version();
    app_object["maintainer"] = container_app->maintainer();
    app_object["description"] = container_app->description();
    for (auto const& status: app_status_names)
    {
      if (container_app->app_status() == status.enumeration)
      {
        app_object["appStatus"] = status.string;
        break;
      }
    }
    apps.append(app_object);
  }
  json_object["installedApps"] = apps;

  return json_object;
}
