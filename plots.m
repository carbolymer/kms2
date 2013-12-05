clear;
load E.dat;
load N.dat;
load X.dat;
load probability.dat;

len = size(probability(1,2:end))(2);
timelen = size(probability(:,1))(1);

middle_probability = probability(round(timelen/2),2:end);

steps = (1:1:len)./len;

figure(1);
plot(E(2:end,1),E(2:end,2));
xlabel('Czas');
ylabel('Energia');
% axis([0 5 4.9 5.04]);
% axis 'autox';
print('E.png','-S640,500');
close(1);

figure(2);
plot(N(:,1),N(:,2));
axis([0 1 0.9 1.1]);
axis 'autox';
xlabel('Czas');
ylabel('Norma');
print('N.png','-S640,500');
close(2);

figure(3);
plot(X(:,1),X(:,2));
ylabel('Srednie polozenie');
xlabel('Czas');
axis([0 1 0 1]);
axis 'autox';
print('X.png','-S640,500');
close(3);

figure(4);
% plot(steps,probability(1,2:end));
plot(steps,probability(1,2:end), steps,middle_probability, steps, probability(end,2:end));
legend('t=0','t=50%','t=100%');
ylabel('Prawdopodobienstwo');
xlabel('Polozenie');
print('P.png','-S640,500');
close(4);

% disp(['Energia koncowa:   ' num2str(E(round(timelen*5/6),2))]);
disp(['Energia koncowa:   ' num2str(max(E(:,2)))]);
