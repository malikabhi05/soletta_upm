obj-$(FLOW_NODE_TYPE_GENERICANALOG) += genericanalog.mod
obj-genericanalog-$(FLOW_NODE_TYPE_GENERICANALOG) := genericanalog.json emg.o genericanalog.o
obj-genericanalog-$(FLOW_NODE_TYPE_GENERICANALOG)-type := flow
obj-genericanalog-$(FLOW_NODE_TYPE_GENERICANALOG)-extra-ldflags = $(shell pkg-config --libs mraa)
