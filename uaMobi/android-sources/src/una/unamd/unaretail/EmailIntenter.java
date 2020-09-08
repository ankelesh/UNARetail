package una.unamd.unaretail;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;
import android.content.Intent;
import android.widget.Toast;

class EmailIntenter extends org.qtproject.qt5.android.bindings.QtActivity
{

    static public void SendIntent(String subject, String txt)
    {
        EmailIntenter e = new EmailIntenter();
               Intent i = new Intent(Intent.ACTION_SEND);
               i.setType("message/rfc822");
               i.putExtra(Intent.EXTRA_SUBJECT, subject);
               i.putExtra(Intent.EXTRA_TEXT   , txt);
               try {
                   e.startActivity(Intent.createChooser(i, "Send mail..."));
               } catch (android.content.ActivityNotFoundException ex) {
                   Toast.makeText(e, "There are no email clients installed.", Toast.LENGTH_SHORT).show();
               }

     }
}
