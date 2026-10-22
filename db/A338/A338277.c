/*
 * A338277  Greatest integer whose square root is less than or equal to
 *          Sum_{j=0..n} sqrt(j).
 * https://oeis.org/A338277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ( sum_{j=0..n} sqrt(j) )^2 )
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 128‑bit integers. */
static inline seq_t sqrt128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    /* highest power of 4 <= x */
    unsigned __int128 bit = (unsigned __int128)1 << 126;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (seq_t)r;
}

seq_t a(idx_t n) {
    /* Choose a scaling factor large enough to keep the error below ½
       in the squared sum.  For n ≤ 10000 the bound below gives an error
       < 0.5. */
    const uint64_t DEN = 30000000000ULL;          /* 3×10¹⁰  */
    const unsigned __int128 DEN_SQR = (unsigned __int128)DEN * DEN;   /* 9×10²⁰ */

    unsigned __int128 sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * DEN_SQR;
        seq_t term = sqrt128(prod);      /* floor(DEN * sqrt(k)) */
        sum += (unsigned __int128)term;
    }

    unsigned __int128 sq = sum * sum;        /* (S_scaled)^2          */
    unsigned __int128 res = sq / DEN_SQR;     /* floor( (S_scaled / DEN)^2 ) = floor(S^2) */

    return (seq_t)res;
}
