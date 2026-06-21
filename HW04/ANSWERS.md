# Homework 04 — FFT Tutorial Worksheet Answers

Course: Scientific Computing for Physics Students  
Topic: Fourier Transform / FFT tutorial project  
Reference project: `alexninogh/myfft_tutorial`

---

## Part 1: Before running anything

### 1. In your own words, what does an FFT tell you about a signal?

An FFT tells us which frequencies are present in a sampled signal and how strong each frequency component is. In other words, it converts information from the time domain into the frequency domain.

### 2. What is the difference between sampling rate, Nyquist frequency, and frequency resolution?

The sampling rate, usually written as `fs`, is the number of samples measured per second.

The Nyquist frequency is half of the sampling rate:

```text
f_N = fs / 2
```

It is the highest frequency that can be represented correctly without aliasing.

The frequency resolution is the spacing between FFT frequency bins:

```text
Delta f = fs / N = 1 / T
```

where `N` is the number of samples and `T` is the total observation time.

### 3. Why is a uniformly spaced time grid important for the FFT routines used in this project?

The standard FFT assumes that the data points are sampled at constant time intervals. If the time grid is not uniform, the FFT bin frequencies are not meaningful for the data, and the spectrum can be wrong.

---

## Part 2: Build and run

After running:

```bash
make run
```

### 1. Which CSV files are created in `output/`?

The expected files are:

```text
output/good_sampling_signal.csv
output/good_sampling_spectrum.csv
output/undersampled_signal.csv
output/undersampled_spectrum.csv
output/short_record_signal.csv
output/short_record_spectrum.csv
output/coupled_oscillators_time.csv
output/coupled_oscillators_spectrum.csv
```

### 2. Which program studies pure sampling issues?

```text
src/sampling_demo.c
```

### 3. Which program studies a mechanics problem?

```text
src/coupled_oscillators_fft.c
```

### 4. What two frequencies are reported in the well-sampled case?

The well-sampled case contains approximately:

```text
50 Hz and 120 Hz
```

---

## Part 3: Sampling and aliasing

### 1. In the undersampled case, the sampling frequency is `fs = 128 Hz`.

Given.

### 2. Using `f_N = fs / 2`, what is the Nyquist frequency?

```text
f_N = 128 / 2 = 64 Hz
```

### 3. Why can a `120 Hz` component not be reconstructed faithfully here?

Because `120 Hz` is larger than the Nyquist frequency `64 Hz`. Therefore, the sampled data cannot represent it correctly, and the frequency appears as a false lower-frequency component.

### 4. The program reports an aliased peak near `8 Hz`.

Given.

### 5. Explain physically and mathematically why a high-frequency signal can appear as a false low-frequency signal after sampling.

Physically, if we sample too slowly, we do not observe enough points per oscillation to reconstruct the original wave. A fast oscillation can then look like a slower oscillation in the sampled data.

Mathematically, frequencies separated by integer multiples of the sampling frequency can produce the same sampled sequence. For this case:

```text
f_alias = |120 - 128| = 8 Hz
```

So the true `120 Hz` component appears close to `8 Hz` after sampling.

---

## Part 4: Frequency resolution

### 1. The signal contains `50 Hz` and `55 Hz`.

Given.

### 2. Using `T = N Delta t`, what is the total record length?

For the short-record case, the tutorial uses:

```text
N = 64
fs = 512 Hz
```

Since:

```text
Delta t = 1 / fs
```

then:

```text
T = N / fs = 64 / 512 = 0.125 s
```

### 3. What is the frequency spacing of the FFT bins?

```text
Delta f = fs / N = 512 / 64 = 8 Hz
```

Equivalently:

```text
Delta f = 1 / T = 1 / 0.125 = 8 Hz
```

### 4. Why does this make the two frequencies difficult to separate?

The two frequencies are separated by only:

```text
55 Hz - 50 Hz = 5 Hz
```

but the FFT bin spacing is:

```text
8 Hz
```

Because the frequency resolution is coarser than the separation between the two components, the peaks cannot be clearly separated.

### 5. If you wanted to improve the separation, what would you change first?

I would increase the total acquisition time `T`. Since:

```text
Delta f = 1 / T
```

a longer record gives smaller frequency spacing and better frequency resolution. Changing the plotting tool or output file format would not improve the actual numerical resolution.

---

## Part 5: Coupled oscillators

### 1. Write down the equations of motion used in the code.

The equations are:

```text
m x1'' = -(k + kc) x1 + kc x2
m x2'' =  kc x1 - (k + kc) x2
```

where `m` is the mass, `k` is the wall-spring constant, `kc` is the coupling spring constant, and `x1`, `x2` are the displacements.

### 2. What physical system do these equations represent?

They represent two identical masses connected to fixed walls by springs and coupled to each other by another spring.

### 3. What are the two normal modes of this system?

The two normal modes are:

1. In-phase mode: both masses move together.
2. Out-of-phase mode: the masses move in opposite directions.

### 4. Why does the FFT of `x1(t)` show more than one important frequency?

Because the chosen initial condition generally excites a combination of the two normal modes. Since `x1(t)` contains contributions from both modes, its FFT shows more than one important frequency.

### 5. Why is it useful to compare the numerical FFT peaks with the theoretical normal-mode frequencies?

This comparison checks whether the numerical simulation agrees with the analytical physics. If the FFT peaks are far from the theoretical frequencies, this could indicate a mistake in the model, the numerical integration, the FFT calculation, or the interpretation of the output.

---

## Part 6: Plot inspection

### 1. Which plot makes aliasing easiest to see?

The undersampled spectrum plot makes aliasing easiest to see, because the `120 Hz` component appears as a false low-frequency peak near `8 Hz`.

### 2. Which plot makes limited frequency resolution easiest to see?

The short-record spectrum plot makes limited frequency resolution easiest to see, because the two close frequencies, `50 Hz` and `55 Hz`, are not well separated.

### 3. In the coupled-oscillator FFT plot, do the numerical peaks line up well with the theoretical reference frequencies?

Yes. The numerical peaks should line up close to the theoretical normal-mode frequencies.

### 4. If they are not exactly identical, what numerical reasons could explain the small difference?

Possible reasons include finite observation time, discrete FFT bin spacing, numerical integration tolerance, and the fact that the true frequency may not fall exactly on one FFT bin.

---

## Part 7: Small code modification

I choose **Option B: Change the observation time**.

### 1. Increase the number of samples while keeping the interpretation clear.

For example, increase the number of samples in the short-record case from:

```c
N = 64;
```

to:

```c
N = 512;
```

while keeping:

```c
fs = 512.0;
```

### 2. Predict how `Delta f = 1 / T` will change.

Before the change:

```text
T = 64 / 512 = 0.125 s
Delta f = 1 / T = 8 Hz
```

After the change:

```text
T = 512 / 512 = 1 s
Delta f = 1 / T = 1 Hz
```

So the frequency resolution improves from `8 Hz` to `1 Hz`.

### 3. Check whether the `50 Hz` and `55 Hz` peaks become more distinguishable.

With `Delta f = 1 Hz`, the two peaks are separated by five FFT bins. Therefore, the `50 Hz` and `55 Hz` components should become much easier to distinguish.

---

## Part 8: Reflection

From this project, I learned that the FFT result depends strongly on how the data are sampled. In particular, sampling too slowly causes aliasing, while observing for too short a time gives poor frequency resolution. I also learned that scientific software should be organized in a reproducible way, with source files, headers, a Makefile, scripts, and output directories clearly separated. One possible improvement would be to add another example with random noise, because this would make the tutorial closer to real experimental or observational data.

---

## Optional extension: Signal with added random noise

### 1. Model

A noisy sinusoidal signal:

```text
s(t) = sin(2 pi f0 t) + noise
```

### 2. Parameters

```text
f0 = 50 Hz
fs = 512 Hz
N = 1024
noise amplitude = 0.3
```

### 3. Expected spectral signature

The FFT should show a strong peak close to `50 Hz`. The random noise should raise the background level of the spectrum.

### 4. Does the FFT confirm the expectation?

If the noise amplitude is moderate, the `50 Hz` peak should still be visible, but the spectrum will look less clean than the noiseless case.
