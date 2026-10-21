/*
 * A279443  Number of ways to place 9 points on an n X n board so that no more
 *          than 2 points are on a vertical or horizontal straight line.
 * https://oeis.org/A279443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^18 -204*n^16 +1260*n^15 +6846*n^14 -104076*n^13
 *            +394504*n^12 +128520*n^11 -6237075*n^10 +24018372*n^9
 *            -43820196*n^8 +30400020*n^7 +34251148*n^6 -99199296*n^5
 *            +98504496*n^4 -47779200*n^3 +9434880*n^2) / 362880 Note: For the
 *            first four values the numerator is zero
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 24, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Polynomial coefficients for powers 0 .. 18 */
    static const int64_t coeff[19] = {
        0,                 /* n^0  */
        0,                 /* n^1  */
        9434880LL,         /* n^2  */
        -47779200LL,       /* n^3  */
        98504496LL,        /* n^4  */
        -99199296LL,       /* n^5  */
        34251148LL,        /* n^6  */
        30400020LL,        /* n^7  */
        -43820196LL,       /* n^8  */
        24018372LL,        /* n^9  */
        -6237075LL,        /* n^10 */
        128520LL,          /* n^11 */
        394504LL,          /* n^12 */
        -104076LL,         /* n^13 */
        6846LL,            /* n^14 */
        1260LL,            /* n^15 */
        -204LL,            /* n^16 */
        0LL,               /* n^17 */
        1LL                /* n^18 */
    };
    const seq_t denom = (seq_t)362880;

    /* For small seq_t sizes use a signed wider integer to avoid unsigned overflows.
       This is safe for all n where the final result fits into seq_t. */
    if (sizeof(seq_t) <= 8) {
        /* Use unsigned 128‑bit arithmetic (available in C11/C++17 compilers). */
        unsigned __int128 N = (unsigned __int128)n;
        signed __int128 val = 0;

        for (int i = 18; i >= 0; --i) {
            int64_t c = coeff[i];
            if (c >= 0)
                val = val * N + (signed __int128)c;
            else
                val = val * N - (signed __int128)(-c);
        }
        return (seq_t)(val / 362880ULL);
    }

    /* For big integer seq_t types use unsigned arithmetic.
       The expression is non‑negative for n≥1, so summing positive
       and negative terms separately avoids underflow. */
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;
    seq_t pow = (seq_t)1;          /* n^i */
    seq_t N = (seq_t)n;

    for (int i = 0; i <= 18; ++i) {
        int64_t c = coeff[i];
        seq_t term = pow * (seq_t)(c >= 0 ? c : -c);
        if (c >= 0)
            pos += term;
        else
            neg += term;

        if (i < 18)
            pow *= N;
    }

    seq_t numerator = pos - neg;   /* guaranteed non‑negative for n≥1 */
    return numerator / denom;
}
