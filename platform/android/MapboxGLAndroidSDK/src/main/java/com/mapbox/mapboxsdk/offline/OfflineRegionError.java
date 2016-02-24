package com.mapbox.mapboxsdk.offline;

import android.support.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * An Offline Region error
 */
public class OfflineRegionError {

    /**
     * Error code, self-explanatory.
     */
    @IntDef({REASON_SUCCESS, REASON_NOT_FOUND, REASON_SERVER, REASON_CONNECTION, REASON_OTHER})
    @Retention(RetentionPolicy.SOURCE)
    public @interface ErrorReason {}

    public static final int REASON_SUCCESS = 1;
    public static final int REASON_NOT_FOUND = 2;
    public static final int REASON_SERVER = 3;
    public static final int REASON_CONNECTION = 4;
    public static final int REASON_OTHER = 6;

    private @ErrorReason int reason;

    /**
    /* An error message from the request handler, e.g. a server message or a system message
    /* informing the user about the reason for the failure.
     */
    private String message;

    /*
     * Constructors
     */

    private OfflineRegionError() {
        // For JNI use only
    }

    /*
     * Getters
     */

    public @ErrorReason int getReason() {
        return reason;
    }

    public String getMessage() {
        return message;
    }
}
