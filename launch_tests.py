import subprocess
import time

def run_test(executable_path):
    # Launch the C executable
    process = subprocess.Popen(executable_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Wait for 10 seconds or until the process ends
    try:
        process.wait(timeout=10)
    except subprocess.TimeoutExpired:
        # If the process takes more than 10 seconds, handle it here
        print(f"Timeout for {executable_path}")
        return 0  # Failed, so no points

    # Get the return code (return value) of the process
    return_code = process.returncode

    if return_code == 0:
        #print(f"Test passed for {executable_path}")
        return 1  # Passed, so add 1 point
    else:
        print(f"Test failed for {executable_path}")
        return 0  # Failed, so no points

if __name__ == "__main__":
    # Replace "your_executable" with the actual path to your C executables
    executables = [
        "./Testing/testVegetarianBlocked",
        "./Testing/testVegetarianOvertake",
        "./Testing/testMutualExclusionInKitchen",
        "./Testing/testSandwichBoth",
        "./Testing/testSandwichMeat",
        "./Testing/testSandwichTofu",
        "./Testing/testCarnivoreBlocked",
        "./Testing/testCarnivoreOvertake",
        "./Testing/testWaitingSwitch"
    ]
    total_points = 0
    for executable in executables:
        total_points += run_test(executable)

    print(f"Total Points: {total_points} / {len(executables)}")
