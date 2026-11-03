/*
 * A290797  Coefficients in 7-adic expansion of sqrt(-3).
 * https://oeis.org/A290797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth coefficient (0‑based) in the 7‑adic expansion of
 *            √‑3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* The algorithm uses Hensel lifting.  For the current approximation
   x_m = Σ_{i=0}^{m-1} a[i]·7^i  (mod 7^m) we compute the next digit a[m]
   as   a[m] ≡ – ((x_m² + 3)/7^m)·(2·a[0])⁻¹  (mod 7).
   Here (2·a[0])⁻¹ mod 7 = 5 because a[0] = 5.   */

#include <stdlib.h>
#include <stdint.h>

/* Ensure that an array has at least cap elements.  If needed, reallocate
   with double the current size. */
static void grow_size(size_t *cap, void **ptr, size_t elem_size, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap <= min_needed) new_cap <<= 1;
        void *new_ptr = realloc(*ptr, new_cap * elem_size);
        if (!new_ptr) abort();          /* out‑of‑memory – abort */
        if (*ptr == NULL) {
            /* Zero newly allocated space */
            for (size_t i = *cap; i < new_cap; ++i)
                ((unsigned char *)new_ptr)[i * elem_size] = 0;
        } else {
            /* Newly allocated tail is already zero because realloc
               does not initialize it.  Zero it. */
            for (size_t i = *cap; i < new_cap; ++i)
                ((unsigned char *)new_ptr)[i * elem_size] = 0;
        }
        *ptr  = new_ptr;
        *cap  = new_cap;
    }
}

static seq_t *digits = NULL;          /* a[i]   */
static uint64_t *raw = NULL;          /* raw[i] : coefficient before carrying, can be large */
static size_t  d_cap = 0;             /* allocated size for digits   */
static size_t  r_cap = 0;             /* allocated size for raw     */
static idx_t   last_computed = (idx_t)(-1);

seq_t a_wide(idx_t n)
{
    /* ----- initialisation (first call) ----- */
    if (last_computed == (idx_t)(-1)) {
        /* allocate digits array */
        grow_size(&d_cap, (void **)&digits, sizeof(seq_t), (size_t)n+1);
        digits[0] = (seq_t)5;          /* a_wide(0) = 5   */
        last_computed = 0;

        /* allocate raw coefficient array; we need indices up to 2*n */
        grow_size(&r_cap, (void **)&raw, sizeof(uint64_t), 2*(size_t)n + 1);
        raw[0] = (uint64_t)25;         /* 5^2 = 25   */
        /* other raw entries are zero by zeroed allocation */
    }

    /* grow arrays if the caller requested a larger n */
    if ((size_t)n >= d_cap) {
        grow_size(&d_cap, (void **)&digits, sizeof(seq_t), (size_t)n + 1);
    }
    if (2*(size_t)n + 1 >= r_cap) {
        grow_size(&r_cap, (void **)&raw, sizeof(uint64_t), 2*(size_t)n + 1);
    }

    /* ----- generate digits up to n ----- */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        /* m = 0 gives the already known a_wide(0); skip the computation code below. */
        if (m == 0) continue;

        /* ----- compute k = ((x_m^2 + 3) / 7^m) mod 7 ----- */
        uint64_t carry = 3;                     /* +3 at the 7^0 place */
        for (idx_t i = 0; i < m; ++i) {
            carry = (carry + raw[i]) / 7;
        }
        uint64_t k = (raw[m] + carry) % 7;

        /* ----- compute the new digit a[m] ----- */
        const uint64_t inv_two_a0 = 5;          /* (2*a[0])⁻¹ mod 7 */
        uint8_t a_m = (uint8_t)(( (7 - k) * inv_two_a0 ) % 7);
        digits[m] = (seq_t)a_m;

        /* ----- update raw coefficients for the square of the new x ----- */
        for (idx_t i = 0; i < m; ++i) {
            raw[i + m] += 2ULL * a_m * (uint64_t)digits[i];
        }
        raw[2 * m] += (uint64_t)a_m * (uint64_t)a_m;
        /* no need to carry‑reduce raw: the algorithm for k uses raw
           values directly via integer division, so raw can grow large
           but stays well below 2^64 for the range of n we expect. */
    }
    last_computed = n;

    return digits[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
