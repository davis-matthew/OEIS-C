/*
 * A250109  Sequence arising from study of multiplicative complexity of
 *          symmetric functions over a field with characteristic p.
 * https://oeis.org/A250109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(n*(n+1)*(n+2))/6 for n mod 4 == 0 a(n) = -((n+3)*(n^2 +
 *            3*n + 8))/6 for n mod 4 == 1 a(n) = -((n-2)*(n+2)*(n+3))/6 for n
 *            mod 4 == 2 a(n) = -((n+1)*(n+2)*(n+3))/6 for n mod 4 == 3
 * Range:     i8 n <= 8, i16 n <= 56, i32 n <= 2342, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned k = n & 3U;
    unsigned long long num = 0ULL;

    switch (k) {
    case 0: /* n ≡ 0 (mod 4) */
        num = (unsigned long long)n * (n + 1ULL) * (n + 2ULL);
        break;
    case 1: /* n ≡ 1 (mod 4) */
        num = (unsigned long long)(n + 3ULL) *
              ((unsigned long long)n * (unsigned long long)n +
               3ULL * (unsigned long long)n + 8ULL);
        break;
    case 2: /* n ≡ 2 (mod 4) */
        num = (unsigned long long)(n - 2ULL) *
              ((unsigned long long)n + 2ULL) *
              ((unsigned long long)n + 3ULL);
        break;
    case 3: /* n ≡ 3 (mod 4) */
        num = (unsigned long long)(n + 1ULL) *
              ((unsigned long long)n + 2ULL) *
              ((unsigned long long)n + 3ULL);
        break;
    }

    /* the sequence values are negative of num/6 */
    return -(seq_t)(num / 6ULL);
}
