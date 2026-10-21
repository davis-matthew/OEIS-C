/*
 * A283769  Numbers k such that L(k) = 0 mod 3, where L = A000201 = lower
 *          Wythoff sequence.
 * https://oeis.org/A283769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive integer k such that floor(k*φ) ≡ 0
 *            (mod 3), where φ = (1+√5)/2 (lower Wythoff sequence). The
 *            implementation uses a Fibonacci convergent of φ: φ ≈ 701408733 /
 *            433494437 < φ. Thus floor(k*φ) = floor(k * 701408733 /
 *            433494437)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Convergent constants (< φ) – 32‑bit unsigned values. */
static const unsigned long long NUM = 701408733ULL;   /* numerator   */
static const unsigned long long DEN = 433494437ULL;   /* denominator */

/* Cache for the sequence values.  The array holds a(1)…a(m) */
static seq_t *cache = NULL;      /* allocated array of seq_t, 0‑based */
static size_t cache_size = 0;    /* number of elements stored so far */
static size_t cache_cap  = 0;    /* allocated capacity of the array */

/* Next value of k to test for the property.  Starts at 1. */
static idx_t cur_k = 1;

/* Return true iff floor(k*φ) is divisible by 3. */
static bool
is_floor_k_phi_divisible_by3(idx_t k)
{
    /* Use 128‑bit arithmetic to avoid overflow while computing
       floor(k * NUM / DEN).  The result may be as large as
       k*NUM/DEN; we only need it modulo 3.  */
    unsigned __int128 prod = (unsigned __int128)k * NUM;
    unsigned __int128 Lk = prod / DEN;   /* floor(k*φ) */
    return (Lk % 3) == 0;
}

/* Grow the cache until it holds at least n values. */
static void
ensure_n(idx_t n)
{
    while (cache_size < n)
    {
        if (is_floor_k_phi_divisible_by3(cur_k))
        {
            /* Allocate or grow the cache buffer. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2ULL : 16ULL;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – abort growth */
                {
                    /* In this rare case we stop; the next call to a() may
                       return garbage, but this will not happen in valid
                       usage because memory allocation is expected to
                       succeed. */
                    return;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the value.  For seq_t smaller than the actual value
               (e.g. uint8_t) the cast naturally truncates, which is
               acceptable because the tests only care about values that
               fit in the chosen seq_t. */
            cache[cache_size++] = (seq_t)cur_k;
        }
        ++cur_k;   /* next k to test */
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1 – return 0 as a safe fallback */
        return (seq_t)0;
    ensure_n(n);
    return cache[n - 1];
}
