import subprocess
import time
import matplotlib.pyplot as plt

# Port number for the server
port = 8080

# Range for the number of threads
thread_range = [1, 6, 12]

# Range for the concurrency level in Apache Benchmark
concurrency_range = [10, 20, 40, 80]

# Dictionary to hold results for each thread configuration
results = {}

# Run server and benchmarking loop
for num_threads in thread_range:
    results[num_threads] = []
    for concurrency in concurrency_range:
        # Start the cflask server with the specified number of threads
        server_process = subprocess.Popen(
            ["./cflask", str(port), str(num_threads)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        
        # Wait a bit for the server to start
        time.sleep(2)

        # Run Apache Benchmark
        try:
            print(f"Running benchmark for {num_threads} threads and concurrency {concurrency}...")
            ab_output = subprocess.check_output(
                ["ab", "-n", "100", "-c", str(concurrency), f"http://127.0.0.1:{port}/arithmetic/cube?num={num_threads}"],
                stderr=subprocess.PIPE
            ).decode()

            # Extract 'Requests per second' value
            for line in ab_output.splitlines():
                if "Requests per second" in line:
                    rps = float(line.split()[3])  # Assumes format: "Requests per second: 1234.56 [#/sec] (mean)"
                    results[num_threads].append((concurrency, rps))
                    break

        except subprocess.CalledProcessError as e:
            print(f"Benchmarking failed: {e}")

        # Stop the server
        server_process.terminate()
        server_process.wait()
        time.sleep(2)  # Ensure server stops before the next iteration

# Plot the results
plt.figure(figsize=(12, 8))
for num_threads, data in results.items():
    x = [item[0] for item in data]  # concurrency levels
    y = [item[1] for item in data]  # requests per second
    plt.plot(x, y, label=f'{num_threads} threads')

with open('loadtest3.data', 'w') as file:
    for key,value in results.items():
        file.write(f'{key}: {value}\n')

plt.xlabel('Number of Concurrent Requests')
plt.ylabel('Requests per Second')
plt.title('Performance of cflask Server with Varying Thread Pools')
plt.legend()
plt.grid(True)
plt.show()
plt.savefig('loadtest3.png')
