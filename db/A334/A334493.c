/*
 * A334493  a(n) is the "w" part of f(n) = Sum_{k>=0, d_k>0} (1+w)^(d_k-1) *
 *          (3+w)^k where Sum_{k>=0} d_k * 7^k is the base 7 representation of
 *          n and w = -1/2 + sqrt(-3)/2 is a primitive cube root of unity;
 *          sequence A334492 gives "real" parts.
 * https://oeis.org/A334493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = the w‑part of f(n) where * f(n) = Σ_{k≥0, d_k>0}
 *            (1+w)^(d_k-1) · (3+w)^k * and {d_k} are the base‑7 digits of n
 *            (least significant first). * The result is the coefficient of w
 *            in the Eisenstein integer
 * Range:     i8 n <= 8733, i16 n <= 8733, i32 n <= 8733, i64 n <= 8733,
 *            i128 n <= 8733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t u, v; } pair;

static const pair pow1[6] = {
    { 1,  0},  /* (1+w)^0  = 1          */
    { 1,  1},  /* (1+w)^1  = 1+w        */
    { 0,  1},  /* (1+w)^2  = w          */
    { -1, 0},  /* (1+w)^3  = -1         */
    { -1, -1}, /* (1+w)^4  = -1-w       */
    { 0,  -1}  /* (1+w)^5  = -w         */
};

static inline pair mul(pair a, pair b) {
    pair r;
    r.u = a.u * b.u - a.v * b.v;
    r.v = a.u * b.v + a.v * b.u - a.v * b.v;
    return r;
}

/* Computes the w‑part of f(n) */
seq_t a(idx_t n) {
    /* Extract base‑7 digits (least significant first) */
    idx_t temp = n;
    unsigned int digits[64];   /* far more than needed for 64‑bit n */
    int len = 0;
    if (temp == 0) {
        return 0;
    }
    while (temp) {
        digits[len++] = temp % 7;
        temp /= 7;
    }

    /* Prepare (3+w) base and its powers */
    const pair base3 = { 3, 1 };
    pair cur3 = { 1, 0 };  /* (3+w)^0 */

    /* Accumulate result */
    struct { seq_t u, v; } total = { 0, 0 };

    for (int k = 0; k < len; ++k) {
        unsigned int d = digits[k];
        if (d != 0) {
            pair p1 = pow1[d - 1];  /* (1+w)^(d-1) */
            pair prod = mul(p1, cur3);
            total.u += prod.u;
            total.v += prod.v;
        }
        /* advance to next power of (3+w) */
        cur3 = mul(cur3, base3);
    }

    return total.v;  /* w‑part */
}
