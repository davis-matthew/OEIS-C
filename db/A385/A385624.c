/*
 * A385624  Antidiagonal sums of the array defined in A385623.
 * https://oeis.org/A385624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0..n} (i*10^{digits(n-i)} + (n-i)) = n * Σ_{j=0..n}
 *            10^{digits(j)} – Σ_{j=0..n} j*10^{digits(j)} + n*(n+1)/2 where
 *            digits(x) = number of decimal digits of x (digits(0)=1)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 1794, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: number of decimal digits of a non‑negative integer. */
static unsigned digits_u64(uint64_t v)
{
    if (v < 10ULL) return 1;
    if (v < 100ULL) return 2;
    if (v < 1000ULL) return 3;
    if (v < 10000ULL) return 4;
    if (v < 100000ULL) return 5;
    if (v < 1000000ULL) return 6;
    if (v < 10000000ULL) return 7;
    if (v < 100000000ULL) return 8;
    if (v < 1000000000ULL) return 9;
    if (v < 10000000000ULL) return 10;
    if (v < 100000000000ULL) return 11;
    if (v < 1000000000000ULL) return 12;
    if (v < 10000000000000ULL) return 13;
    if (v < 100000000000000ULL) return 14;
    if (v < 1000000000000000ULL) return 15;
    if (v < 10000000000000000ULL) return 16;
    if (v < 100000000000000000ULL) return 17;
    if (v < 1000000000000000000ULL) return 18;
    /* For completeness, though numbers this large are not expected. */
    unsigned d = 19;
    while (v >= 1000000000000000000ULL) {
        v /= 1000000000000000000ULL;
        d += 18;
    }
    return d;
}

/* Leading power of 10 for a given number of decimal digits. */
static seq_t pow10_of_digits(unsigned d)
{
    seq_t p = 1;
    while (d--) p *= 10;
    return p;
}

/* Static cache for prefix sums of 10^{digits(j)} and j*10^{digits(j)}. */
static seq_t *sum10 = NULL;      /* sum_{j=0..i} 10^{digits(j)}   */
static seq_t *sumj10 = NULL;     /* sum_{j=0..i} j*10^{digits(j)}  */
static size_t max_computed = 0;   /* next index requiring computation */

/* Helper to grow a dynamic array to at least needed elements. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t el_size)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;
    void *tmp = realloc(*ptr, new_cap * el_size);
    if (!tmp) {
        /* Allocation failure: abort to avoid undefined behaviour. */
        abort();
    }
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Expand cache if necessary. */
    size_t need = (size_t)n + 1;          /* we need indices 0..n */
    static size_t cap10 = 0, capj10 = 0;
    ensure_capacity(need, &cap10, (void **)&sum10,  sizeof(seq_t));
    ensure_capacity(need, &capj10, (void **)&sumj10, sizeof(seq_t));

    /* Compute prefix sums up to n. */
    for (size_t j = max_computed; j <= (size_t)n; ++j) {
        unsigned d = digits_u64((uint64_t)j);
        seq_t pow10 = pow10_of_digits(d);
        if (j == 0) {
            sum10[0]  = pow10;
            sumj10[0] = 0;
        } else {
            sum10[j]  = sum10[j-1] + pow10;
            sumj10[j] = sumj10[j-1] + ((seq_t)j) * pow10;
        }
    }
    max_computed = (size_t)n + 1;

    seq_t ns = (seq_t)n;
    seq_t res = ns * sum10[n] - sumj10[n] + ns * (ns + 1) / 2;
    return res;
}
