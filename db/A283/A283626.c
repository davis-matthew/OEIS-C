/*
 * A283626  Number of integer triples (x,y,z), x > 0, y > 0, z > 0, such that
 *          x <= p, y <= q, z <= r for any factorization n = p*q*r.
 * https://oeis.org/A283626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered integer triples (x,y,z), x>0, y>0, z>0,
 *            for which there exists a factorisation n = p*q*r with x ≤ p, y ≤
 *            q, z ≤ r (any permutation of the factors is allowed). The
 *            implementation enumerates all factorizations of n and then all
 *            pairs (x,y) with x*y ≤ n, determining the grea...
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t _factor_triples_size = 0;     /* capacity of the array */
static struct Factor {
    idx_t a, b, c;                         /* sorted triple a ≤ b ≤ c */
} *_factor_triples = NULL;

/* Utility to ensure the factor triple array can hold at least 'need' items */
static void _ensure_fact_triples(idx_t need)
{
    if (need <= _factor_triples_size) return;
    idx_t new_size = need;
    struct Factor *tmp = (struct Factor*)realloc(_factor_triples,
                                                new_size * sizeof(struct Factor));
    if (!tmp) abort(); /* allocation failure */
    _factor_triples = tmp;
    _factor_triples_size = new_size;
}

/* Build all sorted factor triples of 'n' and store them in the global array. */
static idx_t _build_factor_triples(idx_t n)
{
    idx_t count = 0;
    for (idx_t a = 1; a <= n; ++a) {
        if (a * a * a > n) break;          /* further a will exceed n when cubed   */
        if (n % a != 0) continue;
        for (idx_t b = a; b <= n / a; ++b) {
            if (a * b * b > n) break;
            if ((n / a) % b != 0) continue;
            idx_t c = n / (a * b);
            if (c < b) continue;           /* enforce sorted a ≤ b ≤ c */
            _ensure_fact_triples(count + 1);
            _factor_triples[count].a = a;
            _factor_triples[count].b = b;
            _factor_triples[count].c = c;
            ++count;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Thread‑safe static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n >= cache_size) {
        idx_t new_size = n + 1;              /* 1‑based indexing; 0 unused */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }
    if (cache[n] != 0) return cache[n];

    /* Build factor triples for this n */
    idx_t num_triples = _build_factor_triples(n);

    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t x = 1; x <= n; ++x) {
        idx_t ymax = n / x;
        for (idx_t y = 1; y <= ymax; ++y) {
            idx_t max_z = 0;

            for (idx_t t = 0; t < num_triples; ++t) {
                idx_t p = _factor_triples[t].a;
                idx_t q = _factor_triples[t].b;
                idx_t r = _factor_triples[t].c;

                /* six ordered assignments of (x,y) to (p,q,r) */
                if (x <= p && y <= q  && r > max_z) max_z = r;
                if (x <= p && y <= r  && q > max_z) max_z = q;
                if (x <= q && y <= p  && r > max_z) max_z = r;
                if (x <= q && y <= r  && p > max_z) max_z = p;
                if (x <= r && y <= p  && q > max_z) max_z = q;
                if (x <= r && y <= q  && p > max_z) max_z = p;
            }

            total += (seq_t)max_z;
        }
    }

    cache[n] = total;
    return total;
}
