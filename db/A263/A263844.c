/*
 * A263844  Constant term in expansion of n in Fraenkel's exotic ternary
 *          representation.
 * https://oeis.org/A263844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(n+2)r⌋ + ⌊(n+1)r⌋ – 2·⌊nr⌋ , r = 1 – 1/√2 The value
 *            a(n) is the first digit (d₀) in the unique representation of n–1
 *            on the basis of the numerators of the convergents to √2 (1, 3,
 *            7, 17, 41, …). The implementation builds the required
 *            convergents lazily and reconstructs the representation...
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* maximum number of convergents that can safely be generated in 64‑bit */
enum { MAX_P = 128 };

/* array of convergent numerators, p[0] = 1, p[1] = 3, p[i] = 2*p[i-1] + p[i-2] */
static uint64_t p[MAX_P];
static size_t pcnt = 0;  /* number of valid entries in p */

/* ensure that p contains all convergents ≤ needed */
static void ensure_p(uint64_t needed)
{
    if (pcnt == 0) {                 /* lazy initialisation */
        p[0] = 1;
        p[1] = 3;
        pcnt = 2;
    }
    while (pcnt < MAX_P && p[pcnt-1] <= needed) {
        /* compute next convergent; stop if it would overflow */
        __uint128_t val = ( __uint128_t )p[pcnt-1] * 2U + p[pcnt-2];
        if (val > UINT64_MAX) {
            break;   /* cannot generate larger convergents */
        }
        p[pcnt] = (uint64_t)val;
        ++pcnt;
    }
}

/* sequence term a(n),  n ≥ 1 */
seq_t a(idx_t n)
{
    /* work with n-1, which is non‑negative */
    uint64_t m = (uint64_t)n - 1ULL;
    if (m == 0ULL) {
        return (seq_t)0;   /* d₀ is 0 for the representation of 0 */
    }

    /* make sure the necessary convergents are available */
    ensure_p(m);

    /* find the largest index k with p[k] ≤ m */
    size_t k = pcnt - 1;
    while (k > 0 && p[k] > m) {
        --k;
    }

    uint64_t rem = m;
    int prev = -1;          /* digit at a higher index, initially none */
    int d0  = 0;            /* digit at index 0 (lowest) */

    /* traverse the convergents from high to low */
    for (size_t i = k; ; --i) {
        int digit;
        if (prev == 2) {
            digit = 0;                     /* rule: if d_{i+1}=2 ⇒ d_i=0 */
        } else {
            digit = (int)(rem / p[i]);     /* 0 ≤ digit ≤ 2 by construction */
            if (digit > 2) {
                digit = 2;
            }
            rem -= (uint64_t)digit * p[i];
        }

        if (i == 0) {
            d0 = digit;
        }

        prev = digit;
        if (i == 0) {
            break;
        }
    }

    return (seq_t)d0;
}
