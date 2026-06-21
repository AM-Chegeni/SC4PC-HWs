# Homework 04 — Possible Errors / GitHub Issue Drafts

The lecture asks whether there are any errors and suggests opening issues or solutions on the GitHub page of the project.

I checked the worksheet and guided solution conceptually. The main physics and numerical answers are consistent. I did not find a serious scientific error in the provided model answers.

However, I would suggest a few small improvements or possible issues.

---

## Issue 1 — Minor wording typo in `LESSONS.md` / lecture text

### Title
Fix small typo: `strung` / `pourpose`

### Description
There are minor spelling mistakes in the teaching material. For example, `pourpose` should be `purpose`. These do not affect the code, but fixing them would make the tutorial more polished for students.

### Suggested fix
Replace:

```text
pourpose
```

with:

```text
purpose
```

---

## Issue 2 — Clarify alias formula for beginners

### Title
Clarify how the alias frequency is selected

### Description
The solution gives the alias example:

```text
f_alias = |120 - 128| = 8 Hz
```

This is correct for the tutorial case. However, for beginners it may help to explain that in general one should choose an integer multiple of the sampling frequency so that the alias falls inside the Nyquist interval.

### Suggested addition
Add a short sentence:

```text
In general, choose the integer n such that |f - n fs| falls between 0 and fs/2.
```

---

## Issue 3 — Add a note about spectral leakage

### Title
Mention spectral leakage as a possible reason for imperfect peaks

### Description
The worksheet discusses finite observation time and FFT bin spacing. Another useful beginner concept is spectral leakage, which occurs when the observed signal does not contain an integer number of periods in the finite time window.

### Suggested addition
In Part 6, Question 4, add spectral leakage as another possible reason why FFT peaks may not match theoretical frequencies perfectly.

---

## Issue 4 — Add one noisy-data example

### Title
Add an optional noisy signal example

### Description
The optional extension suggests adding random noise. This would be very useful because real physics data are usually noisy.

### Suggested solution
Add a simple example:

```text
s(t) = sin(2 pi f0 t) + eta(t)
```

where `eta(t)` is random noise. The expected FFT should show a main peak at `f0` plus a raised noise background.
