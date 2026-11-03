/*
 * A343447  Smallest m such that alternating integer 101...101 = A094028(m) is
 *          a multiple of A045572(n), (i.e., integers coprime with 10).
 * https://oeis.org/A343447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = minimal m ≥ 0 such that Σ_{i=0..m} 10^{2i} ≡ 0 (mod
 *            A045572(n)) Sequence: Smallest m such that the alternating
 *            integer 101…101 (=A094028(m)) is a multiple of the n-th integer
 *            coprime to 10 (A045572(n)). a(1) = 0 by convention (1 divides
 *            everything)
 * Range:     u8 n <= 5, u16 n <= 949, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8581 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static inline seq_t nth_coprime(idx_t n)
/* Return the n-th positive integer coprime to 10.
   The sequence of such integers is 1,3,7,9,11,13,17,19,…,
   which consists of blocks of 4 numbers per block of 10 integers. */
{
    idx_t q = (n - 1) / 4;           /* number of full 10‑integer blocks skipped */
    idx_t r = (n - 1) % 4;           /* position inside the current block   */
    seq_t base = (seq_t)q * 10;      /* lowest number of the current block  */
    /* offsets of the coprime numbers inside each block of 10 integers */
    const seq_t offsets[4] = { (seq_t)1, (seq_t)3, (seq_t)7, (seq_t)9 };
    return base + offsets[r];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n<1 is undefined, return 0 harmlessly */
    seq_t k = nth_coprime(n);              /* k = A045572(n) */
    if (k == (seq_t)1) return (seq_t)0;    /* by definition a(1)=0 */

    seq_t r = 1 % k;                       /* S_0 = 1 */
    seq_t pow = 1 % k;                     /* 10^0 mod k */

    idx_t m = 1;
    for (;; ++m) {
        /* pow = 10^{2m} mod k  (each iteration multiplies by 100) */
        pow = mul_small(pow, 100) % k;     /* mul_small works for built‑in or big integers */
        r = (r + pow) % k;                 /* S_m = S_{m-1} + 10^{2m} mod k */
        if (r == 0) return (seq_t)m;
    }
}
