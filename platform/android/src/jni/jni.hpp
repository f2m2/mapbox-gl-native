#pragma once

#include <jni/basics.hpp>
#include <jni/errors.hpp>

#include <jni.h>

namespace jni
   {
    using jboolean = bool;
    using ::jbyte;
    using ::jchar;
    using ::jshort;
    using ::jint;
    using ::jlong;
    using ::jfloat;
    using ::jdouble;

    /* "cardinal indices and sizes" */
    // typedef jint     jsize;

    // Instead of faux-value types that are actually pointer types, we pass
    // and return references to the underlying element type.
    using jobject = std::pointer_traits< ::jobject >::element_type;
    using jclass  = std::pointer_traits< ::jclass  >::element_type;
//    using ::jstring;
//    using ::jarray;
//    using ::jobjectArray;
//    using ::jbooleanArray;
//    using ::jbyteArray;
//    using ::jcharArray;
//    using ::jshortArray;
//    using ::jintArray;
//    using ::jlongArray;
//    using ::jfloatArray;
//    using ::jdoubleArray;
//    using ::jthrowable;
//    using ::jweak;

    using jfieldID  = std::pointer_traits< ::jfieldID   >::element_type;
    using jmethodID = std::pointer_traits< ::jmethodID  >::element_type;


    using ::jvalue;

    inline jvalue MakeAnything( ThingToMake<jvalue>, jboolean z ) { jvalue v; v.z =  z; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jbyte    b ) { jvalue v; v.b =  b; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jchar    c ) { jvalue v; v.c =  c; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jshort   s ) { jvalue v; v.s =  s; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jint     i ) { jvalue v; v.i =  i; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jlong    j ) { jvalue v; v.j =  j; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jfloat   f ) { jvalue v; v.f =  f; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jdouble  d ) { jvalue v; v.d =  d; return v; };
    inline jvalue MakeAnything( ThingToMake<jvalue>, jobject& l ) { jvalue v; v.l = &l; return v; };


/*
    jobjectRefType
    JNINativeMethod
*/

    using ::JavaVM;
    using ::JNIEnv;


    jclass& FindClass(JNIEnv&, const char*);


    struct GlobalRefDeleter
       {
        void operator()(jobject*) const;
        JNIEnv& env;
       };

    template < class T >
    using UniqueGlobalRef = std::unique_ptr< T, GlobalRefDeleter >;

    template < class R >
    UniqueGlobalRef<R> UniqueGlobalRefCast(UniqueGlobalRef<jobject>&& in)
       {
        return UniqueGlobalRef<R>(static_cast<R*>(in.release()),
                                  std::move(in.get_deleter()));
       }


    UniqueGlobalRef<jobject> NewGlobalRef(JNIEnv&, jobject&);

    template < class T >
    UniqueGlobalRef<T> NewGlobalRef(JNIEnv& env, T& t)
       {
        return UniqueGlobalRefCast<T>(NewGlobalRef(env, static_cast<jobject&>(t)));
       }

    void DeleteGlobalRef(JNIEnv&, UniqueGlobalRef<jobject>&&);


    jmethodID& GetMethodID(JNIEnv&, jclass&, const char* name, const char* sig);
    jmethodID& GetStaticMethodID(JNIEnv&, jclass&, const char* name, const char* sig);


    template < class... Args >
    jobject& NewObject(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return *CheckJavaException(env,
            env.NewObjectA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }


    template < class... Args >
    jobject* CallObjectMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallObjectMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jboolean CallBooleanMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallBooleanMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jbyte CallByteMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallByteMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jchar CallCharMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallCharMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jshort CallShortMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallShortMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jint CallIntMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallIntMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jlong CallLongMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallLongMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jfloat CallFloatMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallFloatMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jdouble CallDoubleMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallDoubleMethodA(&obj, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    void CallVoidMethod(JNIEnv& env, jobject& obj, jmethodID& method, Args&&... args)
       {
        env.CallVoidMethodA(&obj, &method,
            MakeArray<jvalue>(std::forward<Args>(args)...).data());
        CheckJavaException(env);
       }


    template < class... Args >
    jobject* CallStaticObjectMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticObjectMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jboolean CallStaticBooleanMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticBooleanMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jbyte CallStaticByteMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticByteMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jchar CallStaticCharMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticCharMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jshort CallStaticShortMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticShortMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jint CallStaticIntMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticIntMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jlong CallStaticLongMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticLongMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jfloat CallStaticFloatMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticFloatMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    jdouble CallStaticDoubleMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        return CheckJavaException(env,
            env.CallStaticDoubleMethodA(&clazz, &method,
                MakeArray<jvalue>(std::forward<Args>(args)...).data()));
       }

    template < class... Args >
    void CallStaticVoidMethod(JNIEnv& env, jclass& clazz, jmethodID& method, Args&&... args)
       {
        env.CallStaticVoidMethodA(&clazz, &method,
            MakeArray<jvalue>(std::forward<Args>(args)...).data());
        CheckJavaException(env);
       }


    jboolean ExceptionCheck(JNIEnv&);


    // UniqueEnv refers to a JNIEnv obtained via AttachCurrentThread, and represents
    // the obligation to close it via DetachCurrentThread. It stores a reference to
    // the JavaVM to which the JNIEnv belongs, to pass to DetachCurrentThread.
    struct JNIEnvDeleter
       {
        void operator()(JNIEnv*) const;
        JavaVM& vm;
       };

    using UniqueEnv = std::unique_ptr<JNIEnv, JNIEnvDeleter>;

    UniqueEnv AttachCurrentThread(JavaVM&);
    void DetachCurrentThread(JavaVM&, UniqueEnv&&);
   }
