/*
 * A293893  Numbers n such that A083722(n) > 1 and A083722(n) occurs later in
 *          A083722.
 * https://oeis.org/A293893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A083722(k) > 1 and A083722(k) =
 *            A083722(m) for some m > k. Method: sieve of greatest prime
 *            factors. A083722(k) is the product of the primes in (gpf(k), k].
 *            Two such products are equal iff the gpf and the largest prime <=
 *            k agree. So k qualifies iff k is composite and a lat...
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t *a293893_terms = NULL;
static size_t a293893_cnt = 0;

static void a293893_build(idx_t L)
{
    idx_t *gpf = (idx_t *)calloc((size_t)L + 1, sizeof(idx_t));
    idx_t *stamp = (idx_t *)calloc((size_t)L + 1, sizeof(idx_t));
    unsigned char *flag = (unsigned char *)calloc((size_t)L + 1, 1);
    size_t cap = 1024, cnt = 0;
    idx_t *terms = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!gpf || !stamp || !flag || !terms) exit(1);

    for (idx_t i = 2; i <= L; ++i)
        if (gpf[i] == 0)
            for (idx_t j = i; j <= L; j += i)
                gpf[j] = i;

    idx_t p = 2;
    for (idx_t q = 3; q <= L; ++q) {
        if (gpf[q] != q) continue;
        for (idx_t j = q - 1; j > p; --j) {
            idx_t g = gpf[j];
            if (stamp[g] == p) flag[j] = 1;
            else { stamp[g] = p; flag[j] = 0; }
        }
        for (idx_t j = p + 1; j < q; ++j) {
            if (flag[j] && gpf[j] < p) {
                if (cnt == cap) {
                    cap *= 2;
                    terms = (idx_t *)realloc(terms, cap * sizeof(idx_t));
                    if (!terms) exit(1);
                }
                terms[cnt++] = j;
            }
        }
        p = q;
    }
    free(gpf);
    free(stamp);
    free(flag);
    free(a293893_terms);
    a293893_terms = terms;
    a293893_cnt = cnt;
}

seq_t a(idx_t n)
{
    static idx_t limit = 0;
    if (limit == 0) limit = 1024;
    while (n > a293893_cnt || a293893_terms == NULL) {
        a293893_build(limit);
        if (n <= a293893_cnt) break;
        limit *= 2;
    }
    return (seq_t)a293893_terms[n - 1];
}
