#import "MGLLocationManagerUtility.h"
#import <UIKit/UIKit.h>

@interface MGLLocationManagerUtility ()

@property (nonatomic) CLLocationManager *standardLocationManager;
@property (nonatomic) BOOL hostAppHasBackgroundCapability;
@property (nonatomic, getter=isUpdatingLocation) BOOL updatingLocation;

@end

@implementation MGLLocationManagerUtility

- (instancetype)init {
    self = [super init];
    if (self) {
        NSArray *backgroundModes = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UIBackgroundModes"];
       _hostAppHasBackgroundCapability = [backgroundModes containsObject:@"location"];
    }
    return self;
}

- (void)startUpdatingLocation {
    [self configurePassiveStandardLocationManager];
    
    if ([self isUpdatingLocation]) {
        return;
    }
    
    if ([CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorized ||
        [CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorizedWhenInUse) {
        [self.standardLocationManager startUpdatingLocation];
        self.updatingLocation = YES;
    }
}

- (void)stopUpdatingLocation {
    if ([self isUpdatingLocation]) {
        self.updatingLocation = NO;
    }
}

#pragma mark - Utilities

- (void)configurePassiveStandardLocationManager {
    if (!self.standardLocationManager) {
        CLLocationManager *standardLocationManager = [[CLLocationManager alloc] init];
        standardLocationManager.delegate = self;
        standardLocationManager.desiredAccuracy = kCLLocationAccuracyThreeKilometers;
        standardLocationManager.distanceFilter = 1;
        if (self.hostAppHasBackgroundCapability && [standardLocationManager respondsToSelector:@selector(allowsBackgroundLocationUpdates)]) {
            standardLocationManager.allowsBackgroundLocationUpdates = YES;
        }
        self.standardLocationManager = standardLocationManager;
    }
}

#pragma mark - CLLocationManagerDelegate

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    switch (status) {
        case kCLAuthorizationStatusNotDetermined:
        case kCLAuthorizationStatusRestricted:
        case kCLAuthorizationStatusDenied:
            [self stopUpdatingLocation];
            break;
        case kCLAuthorizationStatusAuthorized:
            // Also handles kCLAuthorizationStatusAuthorizedAlways
            [self startUpdatingLocation];
            break;
        case kCLAuthorizationStatusAuthorizedWhenInUse:
            if (UIApplication.sharedApplication.applicationState == UIApplicationStateBackground) {
                // Prevent blue status bar when app is not in foreground
                [self stopUpdatingLocation];
            } else {
                [self startUpdatingLocation];
            }
            break;
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    if ([self.delegate respondsToSelector:@selector(locationManagerUtilityDelegate:didUpdateLocations:)]) {
        [self.delegate locationManagerUtilityDelegate:self didUpdateLocations:locations];
    }
}

@end
