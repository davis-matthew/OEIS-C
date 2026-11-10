/*
 * A247786  Numbers k such that A247784(k+1) - A247784(k) = 1.
 * https://oeis.org/A247786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k with A247784(k+1) >= n+1, where A247784(m) =
 *            floor((12m+11)/6/e); the code inverts this floor formula in
 *            integer arithmetic
 * Range:     u8 n <= 188, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 82 terms through n = 2780 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n + 1ULL;
    unsigned long long num = N * 600000ULL;
    /* T = ceil(num / 36788): smallest 12m+11 reaching value N */
    unsigned long long T = (num + 36787ULL) / 36788ULL;
    unsigned long long m = 1ULL;
    if (T > 11ULL) {
        m = (T - 11ULL + 11ULL) / 12ULL; /* ceil((T-11)/12) */
        if (m < 1ULL) m = 1ULL;
    }
    return (seq_t)(m - 1ULL);
}
