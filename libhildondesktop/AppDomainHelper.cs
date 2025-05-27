namespace LibHildonDesktop
{
    using System;
    using System.Reflection;

    public static class AppDomainHelper
    {
        public static void SetAppDomainConfigurationFile(string path)
        {
            AppDomain domain = AppDomain.CurrentDomain;

            if (domain.SetupInformation.ConfigurationFile != null)
                return;

            MethodInfo method = typeof(AppDomain).GetMethod("getSetup", BindingFlags.NonPublic | BindingFlags.Instance);
            if (method == null)
            {
                Console.WriteLine("Unable to get AppDomain.getSetup method via reflection");
                return;
            }

            object setupObj = method.Invoke(domain, null);
            if (setupObj == null)
            {
                Console.WriteLine("AppDomain.getSetup returned null");
                return;
            }

            // set internal field configuration_file via reflection
            FieldInfo configField = setupObj.GetType().GetField("configuration_file", BindingFlags.NonPublic | BindingFlags.Instance);
            if (configField == null)
            {
                Console.WriteLine("Unable to get AppDomainSetup.configuration_file field via reflection");
                return;
            }

            configField.SetValue(setupObj, path);

            Console.WriteLine("AppDomainSetup.ConfigurationFile forcibly set to: " + path);

        }
    }
}