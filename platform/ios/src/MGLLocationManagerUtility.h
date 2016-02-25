#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@protocol MGLLocationManagerUtilityDelegate;

@interface MGLLocationManagerUtility : NSObject <CLLocationManagerDelegate>

@property (nonatomic, weak) id<MGLLocationManagerUtilityDelegate> delegate;

- (void)startUpdatingLocation;
- (void)stopUpdatingLocation;

@end

@protocol MGLLocationManagerUtilityDelegate <NSObject>

@optional
- (void)locationManagerUtilityDelegate:(MGLLocationManagerUtility *)locationManagerUtility didUpdateLocations:(NSArray *)locations;

@end
