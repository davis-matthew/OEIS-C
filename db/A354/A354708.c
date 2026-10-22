/*
 * A354708  a(n) is the number of 9-digit zeroless pandigital numbers that are
 *          divisible by 3^n.
 * https://oeis.org/A354708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Brute-force enumeration of all 9! zeroless pandigital numbers to
 *            compute a(n)
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* powers of 3 up to 3^14 */
static const uint64_t pow3[15] = {
    1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL, 2187ULL, 6561ULL, 19683ULL,
   59049ULL, 177147ULL, 531441ULL, 1594323ULL, 4782969ULL
};
static const uint64_t MOD = pow3[14];

/* cached results for n = 0..14 */
static seq_t counts[15];
static bool ready = false;

/* recursive enumeration of all 9‑digit zeroless pandigital numbers,
   keeping the current modulo MOD at each step. */
static void backtrack(uint64_t mod, unsigned used, seq_t *cnt) {
    if (used == 0x1FFu) {                /* all 9 digits placed */
        for (int n = 0; n < 15; ++n) {
            if ((mod % pow3[n]) == 0)
                cnt[n] += 1;
        }
        return;
    }
    for (int d = 1; d <= 9; ++d) {
        unsigned bit = 1u << (d-1);
        if (!(used & bit)) {
            uint64_t new_mod = (mod * 10 + d) % MOD;
            backtrack(new_mod, used | bit, cnt);
        }
    }
}

/* initialise the cache on first call */
static void init(void) {
    for (int i = 0; i < 15; ++i) counts[i] = 0;
    backtrack(0, 0, counts);
    ready = true;
}

seq_t a(idx_t n) {
    if (!ready) init();
    if (n < 15) return counts[n];
    return 0;
}
