/*
 * A254952  Number of length 5 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of length‑5 arrays over {1,…,n+1} such * that
 *            every leading partial sum is divisible by 2, 3, 5, 7 or 11
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 105, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#define LCM 2310        /* 2*3*5*7*11 */
static const int primes[5] = {2,3,5,7,11};

/* Determines if a residue is acceptable (divisible by at least one prime). */
static inline bool residue_ok(int r)
{
    for (int i = 0; i < 5; ++i)
        if (r % primes[i] == 0)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* below offset – not used */
    seq_t m = (seq_t)n + 1;             /* upper bound of array entries */

    /* Pre‑compute which residues modulo LCM are acceptable. */
    static bool accept[LCM];
    static bool accept_computed = false;
    if (!accept_computed) {
        for (int r = 0; r < LCM; ++r)
            accept[r] = residue_ok(r);
        accept_computed = true;
    }

    /* Count how many a in [1..m] have each residue modulo LCM. */
    static seq_t A[LCM];
    for (int delta = 0; delta < LCM; ++delta) {
        unsigned long long first = (delta == 0) ? LCM : delta; /* smallest a > 0 with this residue */
        if (first > (unsigned long long)m)
            A[delta] = 0;
        else
            A[delta] = (seq_t)((m - first) / LCM + 1);
    }

    /* DP: dp_cur[res] = number of ways to reach sum ≡ res (mod LCM) after
     * a certain number of steps. */
    static seq_t dp_cur[LCM];
    static seq_t dp_next[LCM];
    for (int i = 0; i < LCM; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;                         /* start with sum 0 */

    for (int step = 0; step < 5; ++step) {
        for (int i = 0; i < LCM; ++i) dp_next[i] = 0;
        for (int old = 0; old < LCM; ++old) {
            seq_t ways = dp_cur[old];
            if (ways == 0) continue;
            for (int delta = 0; delta < LCM; ++delta) {
                seq_t add = A[delta];
                if (add == 0) continue;
                int new_res = old + delta;
                if (new_res >= LCM) new_res -= LCM;
                if (!accept[new_res]) continue;
                dp_next[new_res] += ways * add;
            }
        }
        for (int i = 0; i < LCM; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all final residues. */
    seq_t total = 0;
    for (int i = 0; i < LCM; ++i)
        total += dp_cur[i];
    return total;
}
