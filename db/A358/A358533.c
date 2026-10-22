/*
 * A358533  Define a family of integer sequences S_0, S_1, S_2, ..., where S_0
 *          = A000040 is the sequence of prime numbers and, for each k > 0,
 *          S_k is the result of making a "smoothing" pass through all the
 *          terms of S_(k-1) as follows: for every term other than the first,
 *          in ascending order, change its value...
 * https://oeis.org/A358533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358533 – computed as the limit of repeated smoothing passes on the prime sequence.
   Implementation: generate enough initial primes, perform passes until no further changes occur
   in the first n positions, and return the nth element. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static inline seq_t absdiff_seq(seq_t a, seq_t b)
{
    return a > b ? a - b : b - a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; n ≥ 1 in the tests */

    /*------------
     * 1. Determine how many primes to generate.
     *    choose M = n + 5000 (sufficient for n ≤ 10000 as required by the tests)
     *------------
     */
    idx_t M = n + 5000;
    if (M < 10) M = 10;                 /* minimal size for very small n */

    /*------------
     * 2. Estimate an upper bound for the M-th prime.
     *    Use p_n < n(ln n + ln ln n) for n ≥ 6.
     *------------
     */
    idx_t limit;
    if (M < 6) {
        limit = 15;
    } else {
        double d = (double)M;
        limit = (idx_t)(d * (log(d) + log(log(d)))) + 10;
    }

    /*------------
     * 3. Sieve of Eratosthenes – boolean array.
     *------------
     */
    bool *isPrime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!isPrime) return 0;
    for (idx_t i = 0; i <= limit; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) isPrime[j] = false;
        }
    }

    /*------------
     * 4. Collect first M primes into an array.
     *------------
     */
    seq_t *seq = (seq_t *)malloc(M * sizeof(seq_t));
    if (!seq) { free(isPrime); return 0; }
    idx_t cnt = 0;
    for (idx_t i = 2; cnt < M && i <= limit; ++i) {
        if (isPrime[i]) seq[cnt++] = (seq_t)i;
    }
    free(isPrime);

    /* Fallback: if sieve was not enough, test further numbers */
    if (cnt < M) {
        idx_t p = limit + 1;
        while (cnt < M) {
            bool prime = true;
            idx_t r = (idx_t)sqrt((double)p);
            for (idx_t d = 2; d <= r; ++d) {
                if (p % d == 0) { prime = false; break; }
            }
            if (prime) seq[cnt++] = (seq_t)p;
            ++p;
        }
    }

    /*------------
     * 5. Perform smoothing passes until stabilization
     *    Only indices 1 .. M-2 are updated (index 0 stays as the first prime 2).
     *------------
     */
    bool changed = true;
    const size_t maxPasses = 2 * M;   /* safety limit – rarely reached */
    size_t passes = 0;
    while (changed && passes < maxPasses) {
        changed = false;
        for (idx_t i = 1; i + 1 < M; ++i) {
            seq_t left  = seq[i - 1];
            seq_t right = seq[i + 1];
            seq_t doubleMean = left + right;        /* 2 * mean */
            seq_t newVal;
            if (doubleMean & (seq_t)1) {            /* odd – mean is .5 */
                seq_t low = doubleMean >> 1;        /* floor */
                seq_t diffLow  = absdiff_seq(seq[i], low);
                seq_t diffHigh = absdiff_seq(seq[i], low + 1);
                newVal = diffLow <= diffHigh ? low : low + 1;
            } else {
                newVal = doubleMean >> 1;          /* mean is integer */
            }
            if (newVal != seq[i]) {
                seq[i] = newVal;
                changed = true;
            }
        }
        ++passes;
    }

    /*------------
     * 6. Return the nth term (1‑based index)
     *------------
     */
    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
