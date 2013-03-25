
package cn.yo2.aquarium.phoneloc;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/*
 * Usage:
 * String s = PhoneLocation.getLocationFromPhone("13501340000");
 * String[] loc = null;
 * if (s != null && !s.equals(" ")) {
 *     loc = s.split(",");
 * }
 * if (loc != null) {
 *     System.out.println("location code: " + loc[0]);
 *     System.out.println("location name: " + loc[1]);
 * }
 */

public final class PhoneLocation {
    private static final String PHONE_LOC_DATA_FILE_NAME = "phoneloc.dat";
    private static final int DEFAULT_BUFFER_SIZE = 1024 * 4;

    private static boolean _isPhoneLocDataCopied = false;

    private static void copyPhoneLocDataIfNeeded(Context context) {
        if (_isPhoneLocDataCopied) {
            return;
        }

        File phoneLocDataFile = context.getFileStreamPath(PHONE_LOC_DATA_FILE_NAME);

        if (phoneLocDataFile.exists()) {
            return;
        }

        AssetManager am = context.getAssets();
        InputStream input = null;
        OutputStream output = null;
        try {
            input = am.open(PHONE_LOC_DATA_FILE_NAME);
            output = context.openFileOutput(PHONE_LOC_DATA_FILE_NAME, Context.MODE_PRIVATE);
            copy(input, output);
            _isPhoneLocDataCopied = true;
        } catch (IOException e) {
            e.printStackTrace();
            _isPhoneLocDataCopied = false;
            if (phoneLocDataFile.exists()) {
                phoneLocDataFile.delete();
            }
        } finally {
            if (input != null) {
                try {
                    input.close();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
            
            if (output != null) {
                try {
                    output.close();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
    }

    private static long copy(InputStream input, OutputStream output) throws IOException {
        byte[] buffer = new byte[DEFAULT_BUFFER_SIZE];
        long count = 0;
        int n = 0;
        while (-1 != (n = input.read(buffer))) {
            output.write(buffer, 0, n);
            count += n;
        }
        return count;
    }

    static native String getPhoneLocationJni(String phone, String locDataFilePath);

    private static String _phone, _location;

    public synchronized static String getLocationFromPhone(Context context, String num) {
        copyPhoneLocDataIfNeeded(context);
        
        if (num == null) {
            return null;
        }
        
        if (num.equals(_phone)) {
            return _location;
        }
        
        _phone = num;
        
        String locDataFilePath = context.getFileStreamPath(PHONE_LOC_DATA_FILE_NAME).getAbsolutePath();
        
        Slog.d("locDataFilePath: " + locDataFilePath);
        
        _location = getPhoneLocationJni(num, locDataFilePath);
        return _location;
    }

    private static String _getPosFromPhone(Context context, String num, int i) {
        String s = getLocationFromPhone(context, num);
        String[] loc = (s != null) ? s.split(",") : new String[0];
        if (loc.length == 2) {
            return loc[i];
        }
        return null;
    }

    public static String getCityFromPhone(Context context, String num) {
        return _getPosFromPhone(context, num, 1);
    }

    public static String getCodeFromPhone(Context context, String num) {
        return _getPosFromPhone(context, num, 0);
    }
    
    
    static {
        System.loadLibrary("phoneloc-jni");
    }
}
