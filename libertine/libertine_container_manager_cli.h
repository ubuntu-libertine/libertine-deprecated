
#ifndef _LIBERTINE_MANAGER_CLI_H_
#define _LIBERTINE_MANAGER_CLI_H_

#include "libertine/libertine_lxc_manager_wrapper.h"

#include <QtCore/QObject>

class ContainerConfigList;

class LibertineManagerCommandLine
: public QObject
{
  Q_OBJECT

public:
  LibertineManagerCommandLine(QObject* parent = nullptr);

  ~LibertineManagerCommandLine();

  ContainerConfigList* containers;

public slots:
  void
  finishedPackageInstall(QString const& container_id,
                         QString const& package_name,
                         bool result,
                         QString const& error_msg);
};
#endif /* _LIBERTINE_MANAGER_CLI_H_ */
