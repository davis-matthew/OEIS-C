/*
 * A313577  Coordination sequence Gal.4.97.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            -a(n-1)-a(n-2)-a(n-3)-a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+2*a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)-a(n-14)-a(n-15)-a(n-16)-a(n-17)-a(n-18),
 *            initial values: a(0)=1, a(1)=5, a(2)=10, a(3)=14, a(4)=19,
 *            a(5)=25, a(6)=29, a(7)=35, a(8)=39, a(9)=42, a(10)=51, a(11)=52,
 *            a(12)=59, a(13)=64, a(14)=6...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values up to a(18) */
    static const seq_t init_vals[19] = {
        1ULL, 5ULL, 10ULL, 14ULL, 19ULL, 25ULL, 29ULL, 35ULL,
        39ULL, 42ULL, 51ULL, 52ULL, 59ULL, 64ULL, 65ULL,
        77ULL, 76ULL, 84ULL, 88ULL
    };

    if (n < 19)          /* trivial case */
        return init_vals[n];

    /* Sliding window holding the 18 most recent terms.
       pref[0] = a(k-18), pref[17] = a(k-1) at each iteration. */
    seq_t pref[18];
    for (idx_t i = 0; i < 18; ++i)
        pref[i] = init_vals[i + 1];   /* fill with a(1) .. a(18) */

    /* Compute terms from a(19) up to a(n) */
    for (idx_t k = 19; k <= n; ++k) {
        seq_t cur = 0;
        cur -= pref[17]; /* -a(k-1) */
        cur -= pref[16]; /* -a(k-2) */
        cur -= pref[15]; /* -a(k-3) */
        cur -= pref[14]; /* -a(k-4) */
        cur += pref[13]; /* +a(k-5) */
        cur += pref[12]; /* +a(k-6) */
        cur += pref[11]; /* +a(k-7) */
        cur += pref[10]; /* +a(k-8) */
        cur += 2 * pref[9]; /* +2*a(k-9) */
        cur += pref[8];  /* +a(k-10) */
        cur += pref[7];  /* +a(k-11) */
        cur += pref[6];  /* +a(k-12) */
        cur += pref[5];  /* +a(k-13) */
        cur -= pref[4];  /* -a(k-14) */
        cur -= pref[3];  /* -a(k-15) */
        cur -= pref[2];  /* -a(k-16) */
        cur -= pref[1];  /* -a(k-17) */
        cur -= pref[0];  /* -a(k-18) */

        /* Shift the window left by one position */
        for (idx_t i = 0; i < 17; ++i)
            pref[i] = pref[i + 1];
        pref[17] = cur;
    }

    return pref[17];  /* a(n) */
}
