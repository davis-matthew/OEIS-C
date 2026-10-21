/*
 * A286559  Compound filter (the left & right summand of Hofstadter
 *          Q-sequence): a(n) = P(Q(n-Q(n-1)), Q(n-Q(n-2))), where P(n,k) is
 *          sequence A000027 used as a pairing function, with a(1) = a(2) = 0.
 * https://oeis.org/A286559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((x + y - 1)*(x + y - 2))/2 + x where x = Q(n - Q(n-1)),
 *            y = Q(n - Q(n-2)) Q: Hofstadter Q-sequence, Q(1)=Q(2)=1,
 *            Q(n)=Q(n-Q(n-1))+Q(n-Q(n-2)) a(1) = a(2) = 0
 * Range:     u8 n <= 30, u16 n <= 647, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static idx_t *Q_cache = NULL;
static size_t Q_capacity = 0;
static idx_t Q_length = 0;

static void ensure_Q_cache(idx_t n)
{
    if (n <= Q_length) return;
    size_t needed = (size_t)n + 1;
    if (needed > Q_capacity) {
        size_t new_cap = Q_capacity ? Q_capacity * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        idx_t *tmp = (idx_t*)realloc(Q_cache, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        Q_cache = tmp;
        Q_capacity = new_cap;
    }
    for (idx_t i = Q_length + 1; i <= n; ++i) {
        if (i <= 2) {
            Q_cache[i] = 1;
        } else {
            idx_t a = Q_cache[i - Q_cache[i-1]];
            idx_t b = Q_cache[i - Q_cache[i-2]];
            Q_cache[i] = a + b;
        }
    }
    Q_length = n;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    ensure_Q_cache(n);
    idx_t qn1 = Q_cache[n-1];
    idx_t qn2 = Q_cache[n-2];
    idx_t idx1 = n - qn1;
    idx_t idx2 = n - qn2;
    idx_t x = Q_cache[idx1];
    idx_t y = Q_cache[idx2];
    seq_t sum = (seq_t)x + (seq_t)y;
    seq_t prod = (sum - 1) * (sum - 2);
    seq_t res = prod / 2 + (seq_t)x;
    return res;
}
