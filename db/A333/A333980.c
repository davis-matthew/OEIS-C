/*
 * A333980  Partial sums of A337490.
 * https://oeis.org/A333980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} b(k), where b = A337490: b(0)=1; for n>0,
 *            b(n) = gcd(n, Sum_{k<n} b(k)) if that gcd is not already a term,
 *            else b(n) = b(n-1)+n. Computed by direct simulation
 * Range:     u8 n <= 15, u16 n <= 92, u32 n <= 3778, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9289 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* A337490 terms */
    static seq_t *sums = NULL;    /* partial sums */
    static size_t cap = 0;
    static size_t have = 0;       /* number of computed terms */

    if (terms == NULL) {
        cap = 1024;
        terms = mem_new(seq_t, cap);
        sums = mem_new(seq_t, cap);
        terms[0] = 1;
        sums[0] = 1;
        have = 1;
    }

    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t ncap = cap;
        while (ncap < need) ncap *= 2;
        seq_t *nt = mem_new(seq_t, ncap);
        seq_t *ns = mem_new(seq_t, ncap);
        for (size_t i = 0; i < have; ++i) {
            nt[i] = terms[i];
            ns[i] = sums[i];
        }
        mem_free(terms);
        mem_free(sums);
        terms = nt;
        sums = ns;
        cap = ncap;
    }

    for (size_t i = have; i < need; ++i) {
        seq_t g = gcd((seq_t)i, sums[i - 1]);
        bool exists = false;
        for (size_t j = 0; j < i; ++j) {
            if (terms[j] == g) { exists = true; break; }
        }
        seq_t next = exists ? (seq_t)(terms[i - 1] + (seq_t)i) : g;
        terms[i] = next;
        sums[i] = sums[i - 1] + next;
    }
    if (need > have) have = need;

    return sums[(size_t)n];
}
