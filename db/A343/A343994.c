/*
 * A343994  Number of nodes in graph BC(n,2) when the internal nodes are
 *          counted with multiplicity.
 * https://oeis.org/A343994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 10*n^3 + 25*n^2 + 12*n + 12)/4
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 359, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nn = n;          /* promote index to 64-bit for intermediate */
    unsigned long long n2 = nn * nn;
    unsigned long long n3 = n2 * nn;
    unsigned long long n4 = n3 * nn;
    unsigned long long sum = n4 + 10ULL * n3 + 25ULL * n2 + 12ULL * nn + 12ULL;
    return (seq_t)(sum >> 2);           /* divide by 4 and cast back to seq_t */
}
