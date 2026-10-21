/*
 * A274338  The period 10 sequence of the iterated sum of deficient divisors
 *          function (A187793) starting at 52.
 * https://oeis.org/A274338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=52; a(n+1)=∑_{d|a(n), d<a(n)} d if d is deficient. For
 *            deficient n this reduces to a(n+1)=σ(n). For primitive abundant
 *            n: a(n+1)=σ(n)-n
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"

static inline bool is_deficient(seq_t x) {
    seq_t sig = sigma_of((idx_t)x);
    return sig < (x + x);          /* σ(x) < 2×x ⇒ deficient */
}

static inline seq_t sum_deficient_divisors(seq_t n) {
    seq_t sum = 0;
    seq_t sq = floor_sqrt(n);
    idx_t limit = (idx_t)sq;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t di = (seq_t)i;
        if (n % di == 0) {
            seq_t d1 = di;
            seq_t d2 = n / d1;
            if (d1 != n && is_deficient(d1)) sum += d1;
            if (d2 != n && d2 != d1 && is_deficient(d2)) sum += d2;
        }
    }
    return sum;
}

static void ensure_capacity(size_t needed, seq_t **cache, size_t *cap) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    *cache = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset is 1; behavior for n=0 is irrelevant */

    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    if (len < n) {
        /* initialise first term */
        if (len == 0) {
            ensure_capacity(1, &cache, &cap);
            cache[0] = (seq_t)52;
            len = 1;
        }
        /* compute subsequent terms */
        while (len < n) {
            seq_t last = cache[len - 1];
            seq_t next;
            if (is_deficient(last)) {
                next = sigma_of((idx_t)last);   /* σ(last), includes last itself */
            } else {
                next = sum_deficient_divisors(last);
            }
            ensure_capacity(len + 1, &cache, &cap);
            cache[len] = next;
            ++len;
        }
    }
    return cache[n - 1];
}
