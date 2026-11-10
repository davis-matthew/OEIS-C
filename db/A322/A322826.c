/*
 * A322826  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A052126(i) = A052126(j) for all i, j.
 * https://oeis.org/A322826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A052126, where
 *            A052126(n) = n/(greatest prime dividing n), A052126(1) = 1.
 *            Computed by a sieve-like first-occurrence labelling
 * Range:     u8 n <= 5042, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9005 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A052126_val(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t orig = m, largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return orig / largest;
}

seq_t a(idx_t n)
{
    static uint64_t *res = NULL;   /* res[i] = a(i) */
    static uint64_t cap = 0;

    if (n == 0) return 0;
    if ((uint64_t)n > cap) {
        uint64_t nc = cap * 2;
        if (nc < (uint64_t)n) nc = (uint64_t)n;
        if (nc < 1024) nc = 1024;
        uint64_t *nr = (uint64_t *)malloc((nc + 1) * sizeof(uint64_t));
        uint64_t *map = (uint64_t *)calloc(nc + 1, sizeof(uint64_t));
        if (!nr || !map) exit(EXIT_FAILURE);
        uint64_t next = 0;
        nr[0] = 0;
        for (uint64_t i = 1; i <= nc; ++i) {
            uint64_t v = A052126_val(i);
            if (map[v] == 0) map[v] = ++next;
            nr[i] = map[v];
        }
        free(map);
        free(res);
        res = nr;
        cap = nc;
    }
    return (seq_t)res[n];
}
