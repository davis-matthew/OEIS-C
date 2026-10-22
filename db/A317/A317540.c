/*
 * A317540  a(-1) = -1, a(0) = 1, a(n) = A317539(n) - 3*(n-1) for n > 0.
 * https://oeis.org/A317540
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A317540
 *  Defined by piecewise rules:
 *    a(-1) = -1
 *    a(0)  =  1
 *    a(1)  =  2
 *    a(4n) =  1          (n≥0)
 *    a(4n+2) = 0         (n≥0)
 *    a(8n+3) =  2        (n≥0)
 *    a(8n+7) = -1        (n≥0)
 *    a(8n+1) = a(4n+1)   (n≥0)
 *    a(8n+5) = a(4n-1)   (n≥0)
 *
 *  The function below evaluates a(n) for any n ≥ -1 using these rules.
 */

seq_t a(idx_t n)
{
    /* Special base cases */
    if (n == -1)                 return -1;
    if (n == 0)                  return  1;
    if (n == 1)                  return  2;   /* avoid recursion loop */

    /* Even indices */
    if ((n & 1LL) == 0) {        /* n is even */
        if ((n & 3LL) == 0)      /* n % 4 == 0 */
            return 1;
        /* n % 4 == 2 */
        return 0;
    }

    /* Odd indices: handle by mod 8 */
/*  Note: (n & 7LL) is n % 8 for non‑negative n.  */
    switch (n & 7LL) {
        case 3:                     /* n % 8 == 3 */
            return 2;
        case 7:                     /* n % 8 == 7 */
            return -1;
        case 1: {                   /* n % 8 == 1  →  a(4k+1) */
            idx_t k = (n - 1) / 8;  /* k ≥ 0 */
            return a(4 * k + 1);    /* recursion, k < n */
        }
        case 5: {                   /* n % 8 == 5  →  a(4k-1) */
            idx_t k = (n - 5) / 8;  /* k ≥ 0 */
            return a(4 * k - 1);    /* may reach -1 for k==0 */
        }
        default:                     /* should not happen for n≥0 */
            return 0;
    }
}
