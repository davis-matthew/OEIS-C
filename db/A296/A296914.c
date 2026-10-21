/*
 * A296914  List of coefficients of chromatic polynomial of the cubical graph
 *          Q_3, highest order terms first.
 * https://oeis.org/A296914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the coefficient of λ^(9‑n) in the chromatic polynomial
 *            of the 3‑dimensional cube Q_3, i.e. P(Q_3, λ) = λ^8 – 12λ^7 +
 *            66λ^6 – 214λ^5 + 441λ^4 – 572λ^3 + 423λ^2 – 133λ
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are 8 vertices and 12 edges in Q_3.
       coefficient[comp] accumulates Σ(-1)^{|A|} over all
       edge‑subsets A whose induced subgraph has `comp` connected
       components.  `comp` ranges from 1 (connected graph) to 8
       (empty edge set).  coefficient[0] is unused and left at zero. */
    static seq_t coefficient[9] = {0};
    static bool initialized = false;

    if (!initialized)
    {
        /* Edge list of Q_3 (correct 12 edges).  Each vertex has an
           integer label 0..7.                            */
        const int edge[12][2] = {
            {0, 1}, {0, 2}, {0, 4},
            {1, 3}, {1, 5},
            {2, 3}, {2, 6},
            {3, 7},
            {4, 5}, {4, 6},
            {5, 7},
            {6, 7}
        };

        /* Enumerate all 2^12 = 4096 subsets of the edge set. */
        for (int mask = 0; mask < (1 << 12); ++mask)
        {
            int parent[8];
            for (int i = 0; i < 8; ++i) parent[i] = i;

            int bits = 0;  /* number of edges in the current subset */
            for (int e = 0; e < 12; ++e)
            {
                if (mask & (1 << e))
                {
                    ++bits;
                    int u = edge[e][0];
                    int v = edge[e][1];

                    /* Find root of u */
                    int ru = u;
                    while (parent[ru] != ru) ru = parent[ru];

                    /* Find root of v */
                    int rv = v;
                    while (parent[rv] != rv) rv = parent[rv];

                    if (ru != rv) parent[rv] = ru;
                }
            }

            /* Count connected components of the induced subgraph. */
            bool seen[8] = {0};
            int comp = 0;
            for (int i = 0; i < 8; ++i)
            {
                int r = i;
                while (parent[r] != r) r = parent[r];
                if (!seen[r])
                {
                    seen[r] = true;
                    ++comp;
                }
            }

            seq_t sign = (bits % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            coefficient[comp] += sign;
        }

        initialized = true;
    }

    if (n == 0) return 0;          /* outside offset, irrelevant */
    if (n <= 9) {                  /* 1 ≤ n ≤ 9 matches OEIS offset */
        int idx = 9 - (int)n;      /* λ^8 → idx 8, …, λ^0 → idx 0 */
        return coefficient[idx];
    }
    return 0;                      /* higher terms are zero */
}
