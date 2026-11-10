/*
 * A331307  Lexicographically earliest infinite sequence such that for all i,
 *          j: a(i) = a(j) => f(i) = f(j), where f(n) = A285722(n), except
 *          f(1) = -1.
 * https://oeis.org/A331307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = restricted growth sequence transform of f, where f(1) =
 *            -1 and f(n) = A285722(n) for n > 1; computed by scanning m =
 *            1..n and numbering the distinct values of f in order of first
 *            appearance
 * Range:     u8 n <= 397, u16 n <= 25425, u32 n <= 25425, u64 n <= 25425,
 *            u128 n <= 25425, big any n
 * Validated: standard, pass, 116 terms through n = 8452 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A285722_a(idx_t n)
{
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++d;
        sum += d;
    }
    idx_t prev_sum = sum - d;
    idx_t offset = n - prev_sum - 1;

    idx_t i = offset + 1;
    idx_t j = d - offset;

    if (i == j) {
        return (seq_t)0;
    }

    idx_t m, k;
    if (i > j) {
        m = i - j;
        k = j;
    } else {
        m = i;
        k = j - i;
    }

    seq_t m_t = (seq_t)m;
    seq_t k_t = (seq_t)k;
    seq_t sum_mk = m_t + k_t - 2;
    seq_t pair = (sum_mk * (m_t + k_t - 1)) / 2 + m_t;

    return pair;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    size_t cap = (size_t)n;
    seq_t *vals = mem_new(seq_t, cap);
    seq_t *ids = mem_new(seq_t, cap);
    size_t cnt = 0;
    seq_t result = (seq_t)1;

    for (idx_t m = 2; m <= n; m++) {
        seq_t v = A285722_a(m);
        size_t j;
        for (j = 0; j < cnt; j++) {
            if (vals[j] == v) break;
        }
        seq_t id;
        if (j < cnt) {
            id = ids[j];
        } else {
            id = (seq_t)(cnt + 2);
            vals[cnt] = v;
            ids[cnt] = id;
            cnt++;
        }
        result = id;
    }

    mem_free(vals);
    mem_free(ids);
    return result;
}
