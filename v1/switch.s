/*void switch_to(int n)*/

.section .text
.global switch_to
switch_to:
  push %ebp
  mov %esp, %ebp /* 更改栈帧，以便寻参 */

  /* 保存现场 */
	push %edi
	push %esi
	push %ebx
	push %edx
	push %ecx
	push %eax
  pushfl
 
  /* 准备切换栈 */
  mov cur, %eax /* 保存当前 esp */
	mov %esp, task(,%eax,4)
  mov 8(%ebp), %eax /* 取下一个线程 id */
  mov %eax, cur /* 将 cur 重置为下一个线程 id */
	mov task(,%eax,4), %esp /* 切换到下一个线程的栈 */

  /* 恢复现场, 到这里，已经进入另一个线程环境了，本质是 esp 改变 */
  popfl
	popl %eax
	popl %edx
	popl %ecx
	popl %ebx
	popl %esi
	popl %edi

	popl %ebp
  ret


