/*
 * A334492  a(n) is the "real" part of f(n) = Sum_{k>=0, d_k>0} (1+w)^(d_k-1)
 *          * (3+w)^k where Sum_{k>=0} d_k * 7^k is the base 7 representation
 *          of n and w = -1/2 + sqrt(-3)/2 is a primitive cube root of unity;
 *          sequence A334493 gives "w" parts.
 * https://oeis.org/A334492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = real part of Σ_{k≥0, d_k > 0} (1+w)^(d_k-1) * (3+w)^k
 *            where n = Σ d_k * 7^k and w = -1/2 + sqrt(-3)/2 (Eisenstein
 *            integer representation: z = u + v*w, return u)
 * Range:     i8 n <= 7448, i16 n <= 7448, i32 n <= 7448, i64 n <= 7448,
 *            i128 n <= 7448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

typedef struct {
    seq_t u;
    seq_t v;
} pair;

static inline pair mul(pair a, pair b) {
    pair r;
    r.u = a.u * b.u - a.v * b.v;
    r.v = a.u * b.v + a.v * b.u - a.v * b.v;
    return r;
}

static bool ready = false;
static pair pow1_arr[6];
static pair pow3_arr[64];

static void init(void) {
    pair one = {1, 0};
    pair wbase = {1, 1};          /* 1 + w   */
    pair three_w_base = {3, 1};   /* 3 + w   */

    /* (1+w)^e for e = 0..5 */
    pow1_arr[0] = one;
    for (int i = 1; i < 6; ++i) {
        pow1_arr[i] = mul(pow1_arr[i - 1], wbase);
    }

    /* (3+w)^k for k = 0..63 (sufficient for 64‑bit indices) */
    pow3_arr[0] = one;
    for (int k = 1; k < 64; ++k) {
        pow3_arr[k] = mul(pow3_arr[k - 1], three_w_base);
    }

    ready = true;
}

seq_t a(idx_t n) {
    if (!ready) init();

    pair sum = {0, 0};
    idx_t temp = n;
    int k = 0;

    while (temp > 0) {
        int digit = temp % 7;
        if (digit > 0) {
            pair p1 = pow1_arr[digit - 1];   /* (1+w)^(digit-1) */
            pair p2 = pow3_arr[k];           /* (3+w)^k       */
            pair term = mul(p1, p2);
            sum.u += term.u;
            sum.v += term.v;
        }
        temp /= 7;
        ++k;
    }

    return sum.u;
}
