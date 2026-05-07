// RUN: %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir -DBITCODE -emit-llvm-bc -o %t.bc %s
// RUN: %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir -DBITCODE2 -emit-llvm-bc -o %t-2.bc %s
// RUN: %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir -mlink-bitcode-file %t.bc \
// RUN:     -emit-llvm -o - %s | FileCheck -check-prefix=CHECK-BC %s
// RUN: %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir -mlink-builtin-bitcode %t.bc \
// RUN:     -emit-llvm -o - %s | FileCheck -check-prefix=CHECK-BUILTIN-BC %s
// RUN: %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir -emit-llvm -o - \
// RUN:     -mlink-bitcode-file %t.bc -mlink-bitcode-file %t-2.bc %s \
// RUN:     | FileCheck -check-prefix=CHECK-BC -check-prefix=CHECK-BC2 %s
// RUN: not %clang_cc1 -O1 -triple x86_64-unknown-linux-gnu -fclangir \
// RUN:     -mlink-bitcode-file no-such-file.bc -emit-llvm -o - %s 2>&1 \
// RUN:     | FileCheck -check-prefix=CHECK-NO-FILE %s

int f(void);

#ifdef BITCODE

extern int f2(void);

int f(void) {
  f2();
  return 42;
}

#elif defined(BITCODE2)

int f2(void) { return 43; }

#else

// CHECK-BC-LABEL: define{{.*}} i32 @g
// CHECK-BC: ret i32 42
// CHECK-BUILTIN-BC-LABEL: define{{.*}} i32 @g
// CHECK-BUILTIN-BC: ret i32 42
int g(void) {
  return f();
}

// CHECK-BC-LABEL: define{{.*}} i32 @f
// CHECK-BC2-LABEL: define{{.*}} i32 @f2

#endif

// CHECK-NO-FILE: fatal error: cannot open file 'no-such-file.bc'
