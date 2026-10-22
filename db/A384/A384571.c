/*
 * A384571  Consecutive states of the linear congruential pseudo-random number
 *          generator (4160984121*s+1) mod 10^10 when started at s=1.
 * https://oeis.org/A384571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4160984121 * a(n-1) + 1) mod 10^10, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mod_mul(const seq_t a, const seq_t b, const seq_t mod) {
    seq_t res = 0;
    seq_t aa = a;
    if (aa >= mod) aa -= mod;
    seq_t bb = b;
    while (bb) {
        if (bb & 1) {
            res += aa;
            if (res >= mod) res -= mod;
        }
        aa += aa;
        if (aa >= mod) aa -= mod;
        bb >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;                /* a(1) = 1, undefined for n=0 */

    const seq_t MOD = (seq_t)10000000000ULL;    /* 10^10 */
    const seq_t MUL = (seq_t)4160984121ULL;     /* recurrence multiplier */

    /* Represent matrices [[x,y],[0,1]] by the pair (x,y) */
    seq_t r_x = 1, r_y = 0;        /* identity matrix */
    seq_t b_x = MUL % MOD, b_y = 1;/* base matrix T = [[MUL,1],[0,1]] */

    idx_t exp = n - 1;
    while (exp) {
        if (exp & 1) {
            seq_t new_r_x = mod_mul(r_x, b_x, MOD);
            seq_t new_r_y = mod_mul(r_x, b_y, MOD);
            new_r_y += r_y;
            if (new_r_y >= MOD) new_r_y -= MOD;
            r_x = new_r_x; r_y = new_r_y;
        }

        /* square base matrix: [[b_x,b_y],[0,1]]^2 = [[b_x^2, b_x*b_y+b_y],[0,1]] */
        seq_t new_b_x = mod_mul(b_x, b_x, MOD);
        seq_t new_b_y = mod_mul(b_x, b_y, MOD);
        new_b_y += b_y;
        if (new_b_y >= MOD) new_b_y -= MOD;
        b_x = new_b_x; b_y = new_b_y;

        exp >>= 1;
    }

    /* a(n) = r_x * a(1) + r_y  (a(1)=1) */
    seq_t ans = r_x;
    ans += r_y;
    if (ans >= MOD) ans -= MOD;
    return ans;
}
