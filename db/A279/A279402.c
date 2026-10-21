/*
 * A279402  Domination number for queen graph on an n X n toroidal board.
 * https://oeis.org/A279402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = \begin{cases} \text{if } n\equiv0\pmod3:\; k = n/3,
 *            \begin{cases} k &\text{if } k\equiv1,5,7,11\pmod{12}\\ k+1
 *            &\text{if } k\equiv2,10\pmod{12}\\ k+2 &\text{otherwise}
 *            \end{cases}\\[6pt] \text{else: } \begin{cases} 5 &\text{if }
 *            n=11\\ \lceil n/2\rceil &\text{otherwise} \end{cases} \end{ca...
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle multiples of 3 */
    if (n % 3ULL == 0ULL) {
        idx_t k = n / 3ULL;           /* k is n / 3 */
        idx_t mod = k % 12ULL;        /* k modulo 12 */
        if (mod == 1ULL || mod == 5ULL ||
            mod == 7ULL || mod == 11ULL) {
            return (seq_t)k;          /* n==1,5,7,11 (mod 12) -> a=k */
        } else if (mod == 2ULL || mod == 10ULL) {
            return (seq_t)(k + 1ULL); /* n==2,10 (mod 12) -> a=k+1 */
        } else {
            return (seq_t)(k + 2ULL); /* otherwise a=k+2 */
        }
    }

    /* handle non‑multiples of 3 */
    if (n == 11ULL) {
        return (seq_t)5ULL;           /* special case */
    }
    /* ceil(n/2) = (n+1)/2 for unsigned integers */
    return (seq_t)((n + 1ULL) / 2ULL);
}
