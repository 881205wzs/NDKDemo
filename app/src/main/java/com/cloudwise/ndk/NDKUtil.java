package com.cloudwise.ndk;
import android.util.Log;

public class NDKUtil {

    // 静态注册Native方法
    public native static String stringFromJNI();

    // 静态注册Native方法
    public native static void setJNILogEnable(int type);

    // 动态注册Native方法
    public native static int getVersionCode();

    // 动态注册Native方法
    public native static String getVersion(int code);

    // 动态注册Native方法，并且C回调Java方法
    public native static void callJavaString();

    // 动态注册Native方法，并且C回调Java方法
    public native static void callJavaVoid();


    // C调用Java方法（带返回值）
    public static String getStringToC(String name){
        Log.e("CLOUDWISE", "name : " + name);
        return "C From Java";
    }

    // C调用Java方法（不带返回值）
    public static void getVoidToC(int id, String name){
        Log.e("CLOUDWISE", "id : " + id + " ---- name : " + name);
    }
}
