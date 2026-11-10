/*
 * A331594  Number of prime factors (with multiplicity) of A331598(n), where
 *          A331598(n) = A122111(n) / gcd(A122111(n),A241909(n)).
 * https://oeis.org/A331594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A331598(n)), where A331598(n) =
 *            A122111(n)/gcd(A122111(n),A241909(n)). Computed on prime-index
 *            exponent vectors: a(n) = Sum_k max(0, e_k(A122111(n)) -
 *            e_k(A241909(n)))
 * Range:     u8 n <= 3241, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9495 (2026-11-08)
 * Helpers:   factor_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi_u64.h"

static int A331594_cmp_idx(const void *x, const void *y)
{
    idx_t va = *(const idx_t *)x;
    idx_t vb = *(const idx_t *)y;
    return (va > vb) - (va < vb);
}

/* Sorted prime indices of n with multiplicity. Returns count. */
static size_t A331594_indices(idx_t n, idx_t *indices)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0, total = 0;
    if (n <= 1) return 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        idx_t id = (idx_t)prime_pi_u64(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) indices[total++] = id;
    }
    qsort(indices, total, sizeof(idx_t), A331594_cmp_idx);
    return total;
}

seq_t a(idx_t n)
{
    idx_t ind[130];
    uint64_t ex122[132], ex241[132];
    size_t cnt = A331594_indices(n, ind);
    for (size_t i = 0; i < 132; ++i) { ex122[i] = 0; ex241[i] = 0; }
    if (cnt == 0) return (seq_t)0;

    /* A122111(n): exponent vector by prime index */
    {
        idx_t remaining = (idx_t)cnt;
        idx_t last_part = 1;
        size_t pos = 0;
        while (pos < cnt) {
            idx_t current = ind[pos];
            size_t freq = 0;
            while (pos < cnt && ind[pos] == current) { ++freq; ++pos; }
            ex122[remaining] += (uint64_t)(current - last_part + 1);
            remaining -= (idx_t)freq;
            last_part = current + 1;
        }
    }

    /* A241909(n): exponent vector by prime index */
    if (cnt == 1) {
        ex241[1] = (uint64_t)ind[0];
    } else {
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t e;
            if (j == 0) e = (uint64_t)(ind[0] - 1);
            else if (j == cnt - 1) e = (uint64_t)(1 + ind[j] - ind[j - 1]);
            else e = (uint64_t)(ind[j] - ind[j - 1]);
            ex241[j + 1] += e;
        }
    }

    uint64_t sum = 0;
    for (size_t k = 1; k <= cnt; ++k)
        if (ex122[k] > ex241[k]) sum += ex122[k] - ex241[k];
    return (seq_t)sum;
}
