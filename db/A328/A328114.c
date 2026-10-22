/*
 * A328114  Maximal digit value used when n is written in primorial base (cf.
 *          A049345).
 * https://oeis.org/A328114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal digit in the primorial base representation of n
 *            (digits are bounded by the next prime – 1). Compute by greedy
 *            base conversion
 * Range:     u8 n <= 919, u16 n <= 8651, u32 n <= 8651, u64 n <= 8651,
 *            u128 n <= 8651, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Minimal prime generator – keeps a list of primes in ascending order.
 *  It generates primes on demand, only up to the number required for the
 *  current n.  The list is small (≤128 entries) which is sufficient for
 *  all test cases involving n ≤ 10 000 and far beyond for realistic use.
 */
static uint64_t _prime_list[128];
static int _prime_cnt = 0;

static void _ensure_prime(int idx)
{
    /* Generate primes until we have at least idx+1 primes. */
    while (_prime_cnt <= idx) {
        uint64_t cand = (_prime_cnt == 0) ? 2 : _prime_list[_prime_cnt - 1] + 1;
        if (cand > 2 && (cand & 1U) == 0) cand++;       /* skip evens >2 */
        for (;;) {
            uint64_t sq = cand * cand;
            int is_prime = 1;
            for (int i = 0; i < _prime_cnt; ++i) {
                if ((uint64_t)_prime_list[i] * _prime_list[i] > cand) break;
                if (cand % _prime_list[i] == 0) { is_prime = 0; break; }
            }
            if (is_prime) break;
            cand += 2;   /* next odd candidate */
        }
        _prime_list[_prime_cnt++] = cand;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  It builds the necessary primorials up to
 *  the largest Primorial ≤ n, then processes the digits from most
 *  significant to least significant, tracking the maximum digit.
 */
seq_t a(idx_t n)
{
    /* Work on a copy of n in the result type. */
    seq_t rem = (seq_t)n;

    /* Build primorials: prim[0] = 1, prim[k] = product of first k primes. */
    static seq_t prim[128];
    static int prim_cnt = 0;
    if (prim_cnt == 0) {
        prim[0] = (seq_t)1;
        prim_cnt = 1;
    }

    /* Extend primorial array until the next primorial would exceed n. */
    while (prim[prim_cnt - 1] <= rem) {
        int prime_idx = prim_cnt - 1;     /* prime for this new level */
        _ensure_prime(prime_idx);
        seq_t next = prim[prim_cnt - 1] * (seq_t)_prime_list[prime_idx];
        prim[prim_cnt++] = next;
    }

    /* Determine the highest index with prim <= n. */
    int last = prim_cnt - 1;             /* prim[last] > n or never extended */
    if (prim[last] > rem && last > 0) last--;

    seq_t maxdigit = 0;
    /* Greedy conversion from most significant primorial downwards. */
    for (int i = last; i >= 0; --i) {
        seq_t digit = rem / prim[i];
        if (digit > maxdigit) maxdigit = digit;
        rem -= digit * prim[i];
    }

    return maxdigit;
}
