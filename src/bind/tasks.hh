/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

#pragma once

#include <misc/fwd.hh>
#include <task/libtask.hh>

namespace bind
{
  namespace tasks
  {
    TASK_GROUP("3. Binding");

    TASK_DECLARE("bound",
                 "Binding with objects.",
                 bound,
                 "bindings-compute");

    TASK_DECLARE("b|bindings-compute",
                 "Binding without objects.",
                 bind,
                 "parse");

    TASK_DECLARE("B|bindings-display",
                 "Display binded tree.",
                 bind_display,
                 "");
    TASK_DECLARE("rename", "display all variables/fonction renamed.", rename, "bindings-compute");

  } // namespace tasks
} // namespace bind
