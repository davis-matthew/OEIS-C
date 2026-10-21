/*
 * A284842  Number of elements of order n in simple Mathieu group M_12.
 * https://oeis.org/A284842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in the simple Mathieu group
 *            M_12. The group has order 95040 and contains elements of orders
 *            1,2,3,4,5,6,8,10,11 only. For any other n the count is zero
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
        case 1:  return (seq_t)1;
        case 2:  return (seq_t)891;
        case 3:  return (seq_t)4400;
        case 4:  return (seq_t)5940;
        case 5:  return (seq_t)9504;
        case 6:  return (seq_t)23760;
        case 8:  return (seq_t)23760;
        case 10: return (seq_t)9504;
        case 11: return (seq_t)17280;
        default: return (seq_t)0;
    }
}
