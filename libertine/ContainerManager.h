/**
 * @file ContainerManager.h
 * @brief Threaded Libertine container manager
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
#ifndef CONTAINER_CONTAINERMANAGER_H_
#define CONTAINER_CONTAINERMANAGER_H_

#include "libertine/libertine_lxc_manager_wrapper.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>

class ContainerManagerWorker
: public QThread
{
  Q_OBJECT
  Q_ENUMS(ContainerAction)
  Q_PROPERTY(ContainerAction containerAction READ container_action WRITE container_action NOTIFY containerActionChanged)
  Q_PROPERTY(QString containerId READ container_id WRITE container_id NOTIFY containerIdChanged)
  Q_PROPERTY(QString containerType READ container_type WRITE container_type NOTIFY containerTypeChanged)
  Q_PROPERTY(QString data READ data WRITE data NOTIFY dataChanged)

public:
  enum class ContainerAction
  : int
  {
    Create,
    Destroy,
    Install,
    Remove,
    Update
  };

public:
  ContainerManagerWorker();
  ContainerManagerWorker(ContainerAction container_action,
                         QString const& container_id,
                         QString const& container_type);
  ContainerManagerWorker(ContainerAction container_action,
                         QString const& container_id,
                         QString const& container_type,
                         QString const& data);
  ~ContainerManagerWorker();

  Q_INVOKABLE void
  CreateContainerManager();

  ContainerAction
  container_action() const;

  void
  container_action(ContainerAction container_action);

  QString const&
  container_id() const;

  void
  container_id(QString const& container_id);

  QString const&
  container_type() const;

  void
  container_type(QString const& container_type);

  QString const&
  data() const;

  void
  data(QString const& data);

protected:
  void run() Q_DECL_OVERRIDE;

private:
  void createContainer(QString const& password);
  void destroyContainer();
  void installPackage(QString const& package_name);
  void removePackage(QString const& package_name);
  void updateContainer();

private:
  ContainerAction container_action_;
  LibertineManagerWrapper *manager_;
  QString container_id_;
  QString container_type_;
  QString data_;

signals:
  void containerActionChanged();
  void containerIdChanged();
  void containerTypeChanged();
  void dataChanged();
  void finished();
  void finishedDestroy(QString const& container_id);
  void finishedInstall(bool result, QString const& error_msg);
  void finishedRemove(bool result, QString const& error_msg);
};

#endif /* CONTAINER_CONTAINERMANAGER_H_ */
