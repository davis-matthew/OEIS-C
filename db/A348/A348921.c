/*
 * A348921  a(n) is the "w" part of f(n) = Sum_{k >= 0} g(d_k) * (4 + w)^k
 *          where g(0) = 0 and g(1 + u + 2*v) = (1 + u) * (1 + w)^v for any u
 *          = 0..1 and v = 0..5, Sum_{k >= 0} d_k * 13^k is the base-13
 *          representation of n and w = -1/2 + sqrt(-3)/2 is a primitive cube
 *          root of unity; sequence A348920 gives "rea...
 * https://oeis.org/A348921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 2196, i16 n <= 2196, i32 n <= 2196, i64 n <= 2196,
 *            i128 n <= 2196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A348921: w part of
   f(n) = Σ_{k≥0} g(d_k) * (4 + w)^k,
   where d_k are the base‑13 digits of n and
   w = -1/2 + sqrt(-3)/2 is a primitive cube root of unity. */

typedef struct { seq_t r; seq_t w; } Pair;
static inline Pair make_pair(seq_t r, seq_t w) { return (Pair){r,w}; }
static inline Pair add(Pair a, Pair b) { return (Pair){a.r + b.r, a.w + b.w}; }
static inline Pair mul(Pair a, Pair b) {
    /* (a_r + a_w w)(b_r + b_w w) = (a_r b_r - a_w b_w)
       + (a_r b_w + a_w b_r - a_w b_w) w */
    seq_t nr = a.r * b.r - a.w * b.w;
    seq_t nw = a.r * b.w + a.w * b.r - a.w * b.w;
    return (Pair){nr, nw};
}

seq_t a(idx_t n)
{
    /* g(d) for d = 0 .. 12 (pre‑computed from the definition) */
    static const Pair g[13] = {
        { (seq_t)0,  (seq_t)0 },   /* d = 0  → g(0) = 0 */
        { (seq_t)1,  (seq_t)0 },   /* d = 1  → u = 0, v = 0  → (1+u)(1+w)^v = 1 */
        { (seq_t)2,  (seq_t)0 },   /* d = 2  → u = 1, v = 0  → 2 */
        { (seq_t)1,  (seq_t)1 },   /* d = 3  → u = 0, v = 1  → (1)(1+w)   */
        { (seq_t)2,  (seq_t)2 },   /* d = 4  → u = 1, v = 1  → 2(1+w)   */
        { (seq_t)0,  (seq_t)1 },   /* d = 5  → u = 0, v = 2  → (1+w)^2   */
        { (seq_t)0,  (seq_t)2 },   /* d = 6  → u = 1, v = 2  → 2(1+w)^2 */
        { (seq_t)-1, (seq_t)0 },   /* d = 7  → u = 0, v = 3  → (1+w)^3   */
        { (seq_t)-2, (seq_t)0 },   /* d = 8  → u = 1, v = 3  → 2(1+w)^3 */
        { (seq_t)-1, (seq_t)-1 },  /* d = 9  → u = 0, v = 4  → (1+w)^4   */
        { (seq_t)-2, (seq_t)-2 },  /* d =10  → u = 1, v = 4  → 2(1+w)^4 */
        { (seq_t)0,  (seq_t)-1 },  /* d =11  → u = 0, v = 5  → (1+w)^5   */
        { (seq_t)0,  (seq_t)-2 }   /* d =12  → u = 1, v = 5  → 2(1+w)^5 */
    };

    const Pair base = { (seq_t)4, (seq_t)1 }; /* 4 + w */
    Pair acc  = { (seq_t)0, (seq_t)0 };      /* accumulator for f(n)   */
    Pair pow  = { (seq_t)1, (seq_t)0 };      /* (4 + w)^k, starting with k = 0 */

    idx_t t = n;
    while (t > 0) {
        idx_t d = t % 13;    /* least significant base‑13 digit */
        t    = t / 13;                           /* next digit in next iteration */

        Pair term = { (seq_t)0, (seq_t)0 };
        if (d != 0) {
            term = mul(g[d], pow);
        }
        acc = add(acc, term);
        pow = mul(pow, base);   /* advance power to (4 + w)^(k+1) */
    }

    return acc.w;   /* return the w‑part of f(n) */
}
