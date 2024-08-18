#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcb440b5e, "module_layout" },
	{ 0x6b6889b6, "seq_read" },
	{ 0xa93bfc46, "remove_proc_entry" },
	{ 0x92997ed8, "_printk" },
	{ 0xe832bcc7, "proc_create" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x1e6d26a8, "strstr" },
	{ 0xe4133be1, "init_task" },
	{ 0xe7a4bdfe, "seq_printf" },
	{ 0x40c7247c, "si_meminfo" },
	{ 0xc1025998, "single_open" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "90006A52A5E4CF4CB5A6906");
