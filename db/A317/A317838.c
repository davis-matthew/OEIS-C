/*
 * A317838  a(n) = Sum_{d|n} A002487(d).
 * https://oeis.org/A317838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A002487(d) (Stern's Diatomic Sequence)
 * Range:     u8 n <= 1049, u16 n <= 9156, u32 n <= 9156, u64 n <= 9156,
 *            u128 n <= 9156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Stern's Diatomic sequence cache (s[1] = 1, s[2] = 1, s[3] = 2, …) */
/* ------------------------------------------------------------------ */
static seq_t *stern_cache = NULL;   /* Stern values, 1‑based */
static idx_t   stern_max   = 0;     /* highest n for which stern_cache[n] is valid */

/* Ensure Stern values are available up to n */
static void ensure_stern(idx_t n)
{
    if (n <= stern_max)
        return;                     /* already computed */

    size_t new_sz = (size_t)n + 1;   /* indices 0 … n, 0 unused */
    stern_cache = (seq_t*)realloc(stern_cache, new_sz * sizeof(seq_t));
    if (!stern_cache)
        abort();                    /* allocation failed */

    if (stern_max == 0) {           /* first time: initialise low values */
        stern_cache[0] = 0;          /* unused slot to keep indices simple */
        stern_cache[1] = 1;
        stern_max = 1;
    }

    for (idx_t i = stern_max + 1; i <= n; ++i) {
        if ((i & 1U) == 0U)          /* even: s[2k] = s[k]          */
            stern_cache[i] = stern_cache[i >> 1];
        else {                       /* odd : s[2k+1] = s[k] + s[k+1] */
            idx_t k = i >> 1;
            stern_cache[i] = stern_cache[k] + stern_cache[k + 1];
        }
    }
    stern_max = n;
}

/* ------------------------------------------------------------------ */
/* Main API: a(n) = Sum_{d|n} Stern(d)                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined in OEIS, but safe fallback */

    ensure_stern(n);

    seq_t sum = 0;                   /* zero of type seq_t */
    idx_t d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += stern_cache[d];
            if (d != n / d)          /* add the paired divisor once */
                sum += stern_cache[n / d];
        }
    }
    return sum;
}
