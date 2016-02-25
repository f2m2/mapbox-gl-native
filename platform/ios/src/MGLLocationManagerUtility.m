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
        [self.standardLocationManager stopUpdatingLocation];
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
        case kCLAuthorizationStatusAuthorized: // Also handles kCLAuthorizationStatusAuthorizedAlways
        case kCLAuthorizationStatusAuthorizedWhenInUse:
            [self startUpdatingLocation];
            break;
        default:
            [self stopUpdatingLocation];
            break;
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    if ([self.delegate respondsToSelector:@selector(locationManagerUtilityDelegate:didUpdateLocations:)]) {
        [self.delegate locationManagerUtilityDelegate:self didUpdateLocations:locations];
    }
}

@end
