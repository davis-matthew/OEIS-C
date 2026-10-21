/*
 * A263635  Sprague-Grundy sequence G(i-Mark({1}, {2})).
 * https://oeis.org/A263635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A263635 – Sprague–Grundy sequence G(i‑Mark({1},{2})).
     The code implements the piecewise conditions given in the OEIS
     definition:
         a(n)=0  iff n=0, n=2, or n≥5 is odd.
         a(n)=1  iff n=1 or n=3
                or n=3⋅2^k for an even k>0
                or n=(2j+1)⋅2^k with an odd k>0 and j>1
                or n=2^k with an odd k≥3
         a(n)=2  otherwise. */

static inline idx_t trailing_zeros(idx_t x)
{
        idx_t u = 0;
        while ((x & 1) == 0) {
                u++;
                x >>= 1;
        }
        return u;
}

seq_t a(idx_t n)
{
        /* n = 0,1,2,3 are handled explicitly. */
        if (n == 0) return (seq_t)0;
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)0;
        if (n == 3) return (seq_t)1;

        /* Odd numbers ≥5 are 0. */
        if (n >= 5 && (n & 1)) return (seq_t)0;

        /* Now n is even and ≥4. */
        /* 1) powers of two with odd exponent ≥3 give 1,
         *    other powers of two give 2. */
        if ((n & (n - 1)) == 0) {          /* n is a power of two */
                idx_t k = trailing_zeros(n);
                if ((k & 1) && k >= 3)     /* odd exponent ≥3 */
                        return (seq_t)1;
                /* n == 2 already handled above; all other pow‑2 → 2 */
                return (seq_t)2;
        }

        /* 2) numbers of the form 3⋅2^k with even k>0 give 1. */
        if ((n % 3) == 0) {
                idx_t m = n / 3;
                if ((m & (m - 1)) == 0) { /* m is a power of two */
                        idx_t k = trailing_zeros(m);
                        if (k > 0 && !(k & 1))   /* even k>0 */
                                return (seq_t)1;
                }
        }

        /* 3) numbers of the form (2j+1)⋅2^k with odd k>0 and j>1 give 1.
           For j>1 the odd part must be ≥5. */
        idx_t k = trailing_zeros(n);
        idx_t odd_part = n >> k;
        if ((k & 1) && odd_part >= 5)      /* odd k>0, odd_part≥5 */
                return (seq_t)1;

        /* 4) all remaining cases give 2. */
        return (seq_t)2;
}
