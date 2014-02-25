# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'variables': {
      'base_target': 0,
      'chromium_root': '<(DEPTH)/third_party/chromium/src',
    },
    'target_conditions': [
      # This part is shared between the targets defined below. Only files and
      # settings relevant for building the Win64 target should be added here.
      # All the rest should be added to the 'base' target below.
      ['base_target==1', {
        'sources': [
        '<(chromium_root)/base/third_party/dmg_fp/g_fmt.cc',
        '<(chromium_root)/base/third_party/dmg_fp/dtoa_wrapper.cc',
        '<(chromium_root)/base/third_party/icu/icu_utf.cc',
        '<(chromium_root)/base/third_party/nspr/prtime.cc',
        '<(chromium_root)/base/at_exit.cc',
        '<(chromium_root)/base/atomicops_internals_x86_gcc.cc',
        '<(chromium_root)/base/callback_internal.cc',
        '<(chromium_root)/base/command_line.cc',
        '<(chromium_root)/base/debug/alias.cc',
        '<(chromium_root)/base/debug/debugger.cc',
        '<(chromium_root)/base/debug/debugger_posix.cc',
        '<(chromium_root)/base/debug/debugger_win.cc',
        '<(chromium_root)/base/debug/stack_trace.cc',
        '<(chromium_root)/base/debug/stack_trace_posix.cc',
        '<(chromium_root)/base/debug/stack_trace_win.cc',
        '<(chromium_root)/base/file_path.cc',
        '<(chromium_root)/base/lazy_instance.cc',
        '<(chromium_root)/base/location.cc',
        '<(chromium_root)/base/logging.cc',
        '<(chromium_root)/base/logging_win.cc',
        '<(chromium_root)/base/mac/foundation_util.mm',
        '<(chromium_root)/base/memory/ref_counted.cc',
        '<(chromium_root)/base/memory/singleton.cc',
        '<(chromium_root)/base/metrics/stats_counters.cc',
        'metrics/stats_table.cc',
        '<(chromium_root)/base/pickle.cc',
        'process_util_posix.cc',
        'process_util_win.cc',
        '<(chromium_root)/base/profiler/alternate_timer.cc',
        '<(chromium_root)/base/profiler/tracked_time.cc',
        '<(chromium_root)/base/safe_strerror_posix.cc',
        '<(chromium_root)/base/string_number_conversions.cc',
        '<(chromium_root)/base/string_piece.cc',
        '<(chromium_root)/base/string_split.cc',
        '<(chromium_root)/base/string_util.cc',
        '<(chromium_root)/base/stringprintf.cc',
        '<(chromium_root)/base/synchronization/condition_variable_posix.cc',
        '<(chromium_root)/base/synchronization/condition_variable_win.cc',
        '<(chromium_root)/base/synchronization/lock.cc',
        '<(chromium_root)/base/synchronization/lock_impl_posix.cc',
        '<(chromium_root)/base/synchronization/lock_impl_win.cc',
        '<(chromium_root)/base/sys_string_conversions_posix.cc',
        '<(chromium_root)/base/sys_string_conversions_mac.mm',
        '<(chromium_root)/base/sys_string_conversions_win.cc',
        '<(chromium_root)/base/threading/platform_thread_mac.mm',
        '<(chromium_root)/base/threading/platform_thread_posix.cc',
        '<(chromium_root)/base/threading/platform_thread_win.cc',
        '<(chromium_root)/base/threading/thread_collision_warner.cc',
        '<(chromium_root)/base/threading/thread_local_posix.cc',
        '<(chromium_root)/base/threading/thread_local_win.cc',
        '<(chromium_root)/base/threading/thread_local_storage_posix.cc',
        '<(chromium_root)/base/threading/thread_local_storage_win.cc',
        '<(chromium_root)/base/threading/thread_restrictions.cc',
        '<(chromium_root)/base/time.cc',
        '<(chromium_root)/base/time_win.cc',
        '<(chromium_root)/base/tracked_objects.cc',
        '<(chromium_root)/base/utf_string_conversion_utils.cc',
        '<(chromium_root)/base/utf_string_conversions.cc',
        '<(chromium_root)/base/vlog.cc',
        '<(chromium_root)/base/win/registry.cc',
        '<(chromium_root)/base/win/win_util.cc',
        '<(chromium_root)/base/win/windows_version.cc',
        ],
        'include_dirs': [
          '<(chromium_root)',
          '<(DEPTH)',
        ],
        # These warnings are needed for the files in third_party\dmg_fp.
        'msvs_disabled_warnings': [
          4244, 4554, 4018, 4102,
        ],
        'mac_framework_dirs': [
          '$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework/Frameworks',
        ],
        'conditions': [
          ['OS != "win"', {
              'sources/': [ ['exclude', '^win/'] ],
            },
          ],
          [ 'OS == "win"', {
              'sources!': [
                '<(chromium_root)/base/string16.cc',
              ],
          },],
        ],
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'base',
      'type': '<(component)',
      'variables': {
        'base_target': 1,
      },
      'dependencies': [
        'base_static',
        '<(DEPTH)/third_party/modp_b64/modp_b64.gyp:modp_b64',
        '<(chromium_root)/base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
      ],
      # TODO(gregoryd): direct_dependent_settings should be shared with the
      #  64-bit target, but it doesn't work due to a bug in gyp
      'direct_dependent_settings': {
        'include_dirs': [
          '<(chromium_root)',
          '<(DEPTH)',
        ],
      },
      'conditions': [
        [ 'OS == "linux"', {
          'cflags': [
            '-Wno-write-strings',
            '-Wno-error',
          ],
          'cflags!': [
            # We turn this flag on in common.gypi (search for "Page Speed"),
            # but it causes problems compiling the base code on 32-bit systems,
            # so turn it off for the base code.  (The issue is that it enables
            # certain stricter error checks, and there's an implicit narrowing
            # conversion in time_posix.cc that it doesn't like.)
            '-std=gnu++0x',
          ],
          'link_settings': {
            'libraries': [
              # We need rt for clock_gettime().
              '-lrt',
            ],
          },
        }],
        [ 'OS == "mac"', {
            'link_settings': {
              'libraries': [
                '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
                '$(SDKROOT)/System/Library/Frameworks/Carbon.framework',
                '$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
                '$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
                '$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
                '$(SDKROOT)/System/Library/Frameworks/Security.framework',
              ],
            },
        },],
      ],
      'sources': [
        '<(chromium_root)/base/string16.cc',
        '<(chromium_root)/base/string16.h',
        '<(chromium_root)/base/time_mac.cc',
        '<(chromium_root)/base/time_posix.cc',
      ],
    },
  ],
}
