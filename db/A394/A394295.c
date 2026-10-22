/*
 * A394295  a(n) is the number of unit cubes in the cubic lattice, divided by
 *          12, that have 2 vertices strictly inside and 6 vertices strictly
 *          outside the sphere (x-1/2)^2 + (y-1/2)^2 + (z-1/2)^2 = n, i.e.,
 *          the sphere centered at the deep hole.
 * https://oeis.org/A394295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 920, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A394295:
   for a given radius squared n, returns the number of unit cubes in the cubic
   lattice that have exactly two vertices strictly inside the sphere
   (x-1/2)^2+(y-1/2)^2+(z-1/2)^2 = n, divided by 12. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* trivial case */
        return (seq_t)0;

    /* 4 * n is the threshold for 4 times the squared distance of a vertex. */
    idx_t thr = n * 4;

    /* Find an integer M such that M^2 <= n < (M+1)^2  (M = floor(sqrt(n))) */
    idx_t maxi = 0;
    while ((maxi + 1) * (maxi + 1) <= n)
        ++maxi;
    int M = (int)(maxi + 2);        /* small safety margin */

    idx_t total = 0;                /* raw count (will be 12 × a(n)) */

    for (int i = -M; i <= M; ++i) {
        idx_t li = ((idx_t)(2 * i - 1)) * ((idx_t)(2 * i - 1));
        idx_t ui = ((idx_t)(2 * i + 1)) * ((idx_t)(2 * i + 1));
        for (int j = -M; j <= M; ++j) {
            idx_t lj = ((idx_t)(2 * j - 1)) * ((idx_t)(2 * j - 1));
            idx_t uj = ((idx_t)(2 * j + 1)) * ((idx_t)(2 * j + 1));
            for (int k = -M; k <= M; ++k) {
                idx_t lk = ((idx_t)(2 * k - 1)) * ((idx_t)(2 * k - 1));
                idx_t uk = ((idx_t)(2 * k + 1)) * ((idx_t)(2 * k + 1));

                int inside = 0;    /* how many vertices are strictly inside */

                /* Enumerate the 8 choices of lower/upper faces */
                for (int a = 0; a < 2; ++a) {
                    idx_t va = a ? ui : li;
                    for (int b = 0; b < 2; ++b) {
                        idx_t vb = b ? uj : lj;
                        idx_t vab = va + vb;
                        for (int c = 0; c < 2; ++c) {
                            idx_t vert = vab + (c ? uk : lk);
                            if (vert < thr) ++inside;
                        }
                    }
                }

                if (inside == 2)
                    ++total;
            }
        }
    }

    /* The raw count is 12 times the sequence value. */
    return (seq_t)(total / 12);
}
